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

#include "ConnectionState.h"

#include <decaf/lang/exceptions/IllegalStateException.h>

using namespace activemq;
using namespace activemq::state;

////////////////////////////////////////////////////////////////////////////////
ConnectionState::ConnectionState( commands::ConnectionInfo* info ) : disposed( false ) {

    this->info.reset( info->cloneDataStructure() );
}

////////////////////////////////////////////////////////////////////////////////
ConnectionState::~ConnectionState() {

    // TODO - Free all pointers.
}

////////////////////////////////////////////////////////////////////////////////
std::string ConnectionState::toString() const {

    if( this->info.get() != NULL ) {
        return this->info->toString();
    }

    return "NULL";
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionState::shutdown() {

    if( this->disposed.compareAndSet( false, true ) ) {

        std::vector<SessionState*> values = this->sessions.values();
        std::vector<SessionState*>::iterator iter = values.begin();

        for( ; iter != values.end(); ++iter ) {
            (*iter)->shutdown();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void ConnectionState::checkShutdown() const {

    if( this->disposed.get() ) {
        throw decaf::lang::exceptions::IllegalStateException(
            __FILE__, __LINE__, "Connection already Disposed" );
    }
}
