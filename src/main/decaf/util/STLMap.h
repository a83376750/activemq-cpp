/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DECAF_UTIL_STLMAP_H_
#define _DECAF_UTIL_STLMAP_H_

#include <map>
#include <vector>
#include <decaf/lang/exceptions/NoSuchElementException.h>
#include <decaf/util/concurrent/Synchronizable.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Map.h>

namespace decaf{
namespace util{

    /**
     * Map template that wraps around a std::map to provide
     * a more user-friendly interface and to provide common
     * functions that do not exist in std::map.
     */
    template <typename K, typename V, typename COMPARATOR = std::less<K> > class STLMap :
        public Map<K, V, COMPARATOR> {
    private:

        std::map<K,V,COMPARATOR> valueMap;
        concurrent::Mutex mutex;

    public:

        /**
         * Default constructor - does nothing.
         */
        STLMap() : Map<K,V,COMPARATOR>() {}

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        STLMap( const STLMap& source ) : Map<K,V,COMPARATOR>() {
            copy( source );
        }

        /**
         * Copy constructor - copies the content of the given map into this
         * one.
         * @param source The source map.
         */
        STLMap( const Map<K,V,COMPARATOR>& source ) : Map<K,V,COMPARATOR>() {
            copy( source );
        }

        virtual ~STLMap() {}

        /**
         * Comparison, equality is dependent on the method of determining
         * if the element are equal.
         * @param source - Map to compare to this one.
         * @returns true if the Map passed is equal in value to this one.
         */
        virtual bool equals( const STLMap& source ) const {
            return this->valueMap == source.valueMap;
        }

        virtual bool equals( const Map<K,V,COMPARATOR>& source ) const {
            std::vector<K> keys = source.keySet();

            typename std::vector<K>::const_iterator iter = keys.begin();
            for( ; iter != keys.end(); ++iter ) {
                if( !this->containsKey( *iter ) ) {
                    return false;
                }

                if( !( this->get( *iter ) == source.get( *iter ) ) ) {
                    return false;
                }
            }

            return true;
        }

        /**
         * Copies the content of the source map into this map.  Erases
         * all existing data in this map.
         * @param source The source object to copy from.
         */
        virtual void copy( const STLMap& source ) {
            this->valueMap.clear();
            this->valueMap.insert( source.valueMap.begin(), source.valueMap.end() );
        }

        virtual void copy( const Map<K,V,COMPARATOR>& source ) {
            this->clear();
            this->putAll( source );
        }

        /**
         * Removes all keys and values from this map.
         */
        virtual void clear() {
            valueMap.clear();
        }

        /**
         * Indicates whether or this map contains a value for the
         * given key.
         * @param key The key to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsKey( const K& key ) const {
            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter = valueMap.find(key);
            return iter != valueMap.end();
        }

        /**
         * Indicates whether or this map contains a value for the
         * given value, i.e. they are equal, this is done by operator==
         * so the types must pass equivalence testing in this manner.
         * @param value The Value to look up.
         * @return true if this map contains the value, otherwise false.
         */
        virtual bool containsValue( const V& value ) const {

            if( valueMap.empty() ){
                return false;
            }

            typename std::map<K,V,COMPARATOR>::const_iterator iter = valueMap.begin();
            for( ; iter != valueMap.end(); ++iter ){
                if( (*iter).second == value ) {
                    return true;
                }
            }

            return false;
        }

        /**
         * @return if the Map contains any element or not, TRUE or FALSE
         */
        virtual bool isEmpty() const {
            return valueMap.empty();
        }

        /**
         * @return The number of elements (key/value pairs) in this map.
         */
        virtual std::size_t size() const {
            return valueMap.size();
        }

        /**
         * Gets the value for the specified key.
         * @param key The search key.
         * @return The value for the given key.
         * @throws NoSuchElementException
         */
        virtual V get( const K& key ) const
            throw( lang::exceptions::NoSuchElementException ) {

            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter = valueMap.find(key);
            if( iter == valueMap.end() ){
                throw lang::exceptions::NoSuchElementException(
                    __FILE__, __LINE__, "Key does not exist in map" );
            }

            return iter->second;
        }

        /**
         * Sets the value for the specified key.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void put( const K& key, V value ) {
            valueMap[key] = value;
        }

        /**
         * Stores a copy of the Mappings contained in the other Map in this one.
         * @param key The target key.
         * @param value The value to be set.
         */
        virtual void putAll( const STLMap<K,V,COMPARATOR>& other ) {
            this->valueMap.insert( other.valueMap.begin(), other.valueMap.end() );
        }
        virtual void putAll( const Map<K,V,COMPARATOR>& other ) {
            std::vector<K> keys = other.keySet();

            typename std::vector<K>::const_iterator iter = keys.begin();
            for( ; iter != keys.end(); ++iter ) {

                this->put( *iter, other.get( *iter ) );
            }
        }

        /**
         * Removes the value (key/value pair) for the specified key from
         * the map.
         * @param key The search key.
         */
        virtual void remove( const K& key ) {
            valueMap.erase( key );
        }

        /**
         * @return the entire set of keys in this map as a std::vector.
         */
        virtual std::vector<K> keySet() const{
            std::vector<K> keys( valueMap.size() );

            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter=valueMap.begin();
            for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                keys[ix] = iter->first;
            }

            return keys;
        }

        /**
         * @return the entire set of values in this map as a std::vector.
         */
        virtual std::vector<V> values() const {
            std::vector<V> values( valueMap.size() );

            typename std::map<K,V,COMPARATOR>::const_iterator iter;
            iter=valueMap.begin();
            for( int ix=0; iter != valueMap.end(); ++iter, ++ix ){
                values[ix] = iter->second;
            }

            return values;
        }

    public:     // Methods from Synchronizable

        /**
         * Locks the object.
         * @throws ActiveMQException
         */
        virtual void lock() throw( lang::Exception ) {
            mutex.lock();
        }

        /**
         * Unlocks the object.
         * @throws ActiveMQException
         */
        virtual void unlock() throw( lang::Exception ) {
            mutex.unlock();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void wait() throw( lang::Exception ) {
            mutex.wait();
        }

        /**
         * Waits on a signal from this object, which is generated
         * by a call to Notify.  Must have this object locked before
         * calling.  This wait will timeout after the specified time
         * interval.
         * @param millisecs the time in milliseconds to wait, or
         * WAIT_INIFINITE
         * @throws ActiveMQException
         */
        virtual void wait( unsigned long millisecs )
            throw( lang::Exception ) {
            mutex.wait(millisecs);
        }

        /**
         * Signals a waiter on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notify() throw( lang::Exception ) {
            mutex.notify();
        }

        /**
         * Signals the waiters on this object that it can now wake
         * up and continue.  Must have this object locked before
         * calling.
         * @throws ActiveMQException
         */
        virtual void notifyAll() throw( lang::Exception ) {
            mutex.notifyAll();
        }
    };

}}

#endif /*_DECAF_UTIL_STLMAP_H_*/
