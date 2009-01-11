/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <activemq/wireformat/openwire/marshal/v1/ExceptionResponseMarshaller.h>

#include <activemq/commands/ExceptionResponse.h>
#include <activemq/exceptions/ActiveMQException.h>

//
//     NOTE!: This file is autogenerated - do not modify!
//            if you need to make a change, please see the Java Classes in the
//            activemq-core module
//

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace activemq::wireformat;
using namespace activemq::wireformat::openwire;
using namespace activemq::wireformat::openwire::marshal;
using namespace activemq::wireformat::openwire::utils;
using namespace activemq::wireformat::openwire::marshal::v1;
using namespace decaf;
using namespace decaf::io;

///////////////////////////////////////////////////////////////////////////////
DataStructure* ExceptionResponseMarshaller::createObject() const {
    return new ExceptionResponse();
}

///////////////////////////////////////////////////////////////////////////////
unsigned char ExceptionResponseMarshaller::getDataStructureType() const {
    return ExceptionResponse::ID_EXCEPTIONRESPONSE;
}

///////////////////////////////////////////////////////////////////////////////
void ExceptionResponseMarshaller::tightUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn, BooleanStream* bs ) throw( decaf::io::IOException ) {

    try {

        ResponseMarshaller::tightUnmarshal( wireFormat, dataStructure, dataIn, bs );

        ExceptionResponse* info =
            dynamic_cast<ExceptionResponse*>( dataStructure );
        info->setException( dynamic_cast< BrokerError* >(
            tightUnmarshalBrokerError( wireFormat, dataIn, bs ) ) );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
int ExceptionResponseMarshaller::tightMarshal1( OpenWireFormat* wireFormat, DataStructure* dataStructure, BooleanStream* bs ) throw( decaf::io::IOException ) {

    try {

        ExceptionResponse* info =
            dynamic_cast<ExceptionResponse*>( dataStructure );

        int rc = ResponseMarshaller::tightMarshal1( wireFormat, dataStructure, bs );
        rc += tightMarshalBrokerError1( wireFormat, info->getException(), bs );

        return rc + 0;
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ExceptionResponseMarshaller::tightMarshal2( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut, BooleanStream* bs ) throw( decaf::io::IOException ) {

    try {

        ResponseMarshaller::tightMarshal2( wireFormat, dataStructure, dataOut, bs );

        ExceptionResponse* info =
            dynamic_cast<ExceptionResponse*>( dataStructure );
        tightMarshalBrokerError2( wireFormat, info->getException(), dataOut, bs );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ExceptionResponseMarshaller::looseUnmarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataInputStream* dataIn ) throw( decaf::io::IOException ) {

    try {

        ResponseMarshaller::looseUnmarshal( wireFormat, dataStructure, dataIn );
        ExceptionResponse* info =
            dynamic_cast<ExceptionResponse*>( dataStructure );
        info->setException( dynamic_cast< BrokerError* >(
            looseUnmarshalBrokerError( wireFormat, dataIn ) ) );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

///////////////////////////////////////////////////////////////////////////////
void ExceptionResponseMarshaller::looseMarshal( OpenWireFormat* wireFormat, DataStructure* dataStructure, DataOutputStream* dataOut ) throw( decaf::io::IOException ) {

    try {

        ExceptionResponse* info =
            dynamic_cast<ExceptionResponse*>( dataStructure );
        ResponseMarshaller::looseMarshal( wireFormat, dataStructure, dataOut );

        looseMarshalBrokerError( wireFormat, info->getException(), dataOut );
    }
    AMQ_CATCH_RETHROW( decaf::io::IOException )
    AMQ_CATCH_EXCEPTION_CONVERT( exceptions::ActiveMQException, decaf::io::IOException )
    AMQ_CATCHALL_THROW( decaf::io::IOException )
}

