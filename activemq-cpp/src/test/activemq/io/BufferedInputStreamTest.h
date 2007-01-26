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

#ifndef ACTIVEMQ_IO_BUFFEREDINPUTSTREAMTEST_H_
#define ACTIVEMQ_IO_BUFFEREDINPUTSTREAMTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/io/BufferedInputStream.h>
#include <activemq/util/Config.h>


namespace activemq{
namespace io{
	
	class BufferedInputStreamTest : public CppUnit::TestFixture {
		
	  CPPUNIT_TEST_SUITE( BufferedInputStreamTest );
	  CPPUNIT_TEST( testSmallerBuffer );
	  CPPUNIT_TEST( testBiggerBuffer );
	  CPPUNIT_TEST_SUITE_END();
	  
	public:
	
		class MyInputStream : public InputStream{
		private:
			std::string data;
			std::size_t pos;
		public:
		
			MyInputStream( const std::string& data ){
				this->data = data;
				pos = 0;
			}
			virtual ~MyInputStream(){}
			
			virtual std::size_t available() const throw (IOException){
				std::size_t len = data.length();
				return len - pos;
			}
			virtual unsigned char read() throw (IOException){
				if( pos >= data.length() ){
					throw IOException();
				}
				
				return data.c_str()[pos++];
			}
            virtual std::size_t read( unsigned char* buffer, std::size_t bufferSize ) throw (IOException){
				std::size_t numToRead = std::min( bufferSize, available() );
				
				const char* str = data.c_str();
				for( std::size_t ix=0; ix<numToRead; ++ix ){
					buffer[ix] = str[pos+ix];
				}
				
				pos += numToRead;
				
				return numToRead;
			}

			virtual void close() throw(cms::CMSException){
				// do nothing.
			}
            virtual std::size_t skip( std::size_t num AMQCPP_UNUSED) throw ( io::IOException, exceptions::UnsupportedOperationException ) {
                return 0;
            }
			
		    virtual void lock() throw(exceptions::ActiveMQException){
		    }
		    virtual void unlock() throw(exceptions::ActiveMQException){
		    }
            virtual void wait() throw(exceptions::ActiveMQException){
            }
            virtual void wait(unsigned long millisecs AMQCPP_UNUSED) throw(exceptions::ActiveMQException){                
            }
            virtual void notify() throw(exceptions::ActiveMQException){
            }
            virtual void notifyAll() throw(exceptions::ActiveMQException){
            }
		};
		
	public:
	
        virtual ~BufferedInputStreamTest(){}
		virtual void setUp(){};	
	 	virtual void tearDown(){};
		void testSmallerBuffer(){
			
			std::string testStr = "TEST12345678910";
			MyInputStream myStream( testStr );
			BufferedInputStream bufStream( &myStream, (std::size_t)1 );
			
			std::size_t available = bufStream.available();
			CPPUNIT_ASSERT( available == testStr.length() );
			
			unsigned char dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'T' );
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == (testStr.length() - 1) );
			
			dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'E' );
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == (testStr.length() - 2 ) );
			
			dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'S' );
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == (testStr.length() - 3 ) );
			
			dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'T' );
			
			unsigned char dummyBuf[20];
			memset( dummyBuf, 0, 20 );
			std::size_t numRead = bufStream.read( dummyBuf, 10 );
			CPPUNIT_ASSERT( numRead == 10 );
			CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );			
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == 1 );
		}
		
		void testBiggerBuffer(){
			
			std::string testStr = "TEST12345678910";
			MyInputStream myStream( testStr );
			BufferedInputStream bufStream( &myStream, (std::size_t)10 );
			
			std::size_t available = bufStream.available();
			CPPUNIT_ASSERT( available == testStr.length() );
			
			unsigned char dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'T' );
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == (testStr.length() - 1 ) );
			
			dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'E' );
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == (testStr.length() - 2 ) );
			
			dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'S' );
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == (testStr.length() - 3 ) );
			
			dummy = bufStream.read();
			CPPUNIT_ASSERT( dummy == 'T' );
			
			unsigned char dummyBuf[20];
			memset( dummyBuf, 0, 20 );
			std::size_t numRead = bufStream.read( dummyBuf, 10 );
			CPPUNIT_ASSERT( numRead == 10 );
			CPPUNIT_ASSERT( strcmp( (char*)dummyBuf, "1234567891" ) == 0 );			
			
			available = bufStream.available();
			CPPUNIT_ASSERT( available == 1 );
		}
	};
	
}}

#endif /*ACTIVEMQ_IO_BUFFEREDINPUTSTREAMTEST_H_*/
