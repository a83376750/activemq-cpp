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
#include <decaf/util/Config.h>

#include "TcpSocket.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "SocketError.h"

using namespace decaf;
using namespace decaf::net;
using namespace decaf::io;
using namespace decaf::lang;

////////////////////////////////////////////////////////////////////////////////
TcpSocket::TcpSocket() throw ( SocketException )
  : socketHandle( INVALID_SOCKET_HANDLE ),
    inputStream( NULL ),
    outputStream( NULL ) {
}

////////////////////////////////////////////////////////////////////////////////
TcpSocket::TcpSocket( SocketHandle socketHandle )
:
    socketHandle( INVALID_SOCKET_HANDLE ),
    inputStream( NULL ),
    outputStream( NULL ) {

    try {

        this->socketHandle = socketHandle;
        this->inputStream = new SocketInputStream( socketHandle );
        this->outputStream = new SocketOutputStream( socketHandle );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
TcpSocket::~TcpSocket() {
    // No shutdown, just close - dont want blocking destructor.
    close();
}

////////////////////////////////////////////////////////////////////////////////
InputStream* TcpSocket::getInputStream(){
    return inputStream;
}

////////////////////////////////////////////////////////////////////////////////
OutputStream* TcpSocket::getOutputStream(){
    return outputStream;
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::connect(const char* host, int port) throw ( SocketException ) {

    try{

        if( isConnected() ) {
            throw SocketException( __FILE__, __LINE__,
                "Socket::connect - Socket already connected.  host: %s, port: %d", host, port );
        }

        // Create the APR Pool
        apr_pool_create( &apr_pool, NULL );

        // Create the Address data
        checkResult( apr_sockaddr_info_get(
            &socketAddress, host, APR_INET, port, 0, apr_pool ) );

        // Create the actual socket.
        checkResult( apr_socket_create(
            &socketHandle, socketAddress->family, SOCK_STREAM, APR_PROTO_TCP, apr_pool ) );

        // it is a good idea to specify socket options explicitly. in this
        // case, we make a blocking socket with system timeout, this should allow us
        // the time needed to connect to the socket before returning, but not
        // block us forever waiting if there isn't anyone there.  Some poeple
        // say that you shouldn't use this mode as its an OS timeout that is not
        // controllable, if this becomes an issue, we should decide on what a
        // reasonable timeout should be and set it here.
        apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, 0 );
        apr_socket_timeout_set( socketHandle, -1 );

        checkResult( apr_socket_connect( socketHandle, socketAddress ) );

        // Now that we are connected, we want to set ourself up as a blocking
        // socket by default.
        apr_socket_opt_set( socketHandle, APR_SO_NONBLOCK, 0 );
        apr_socket_timeout_set( socketHandle, -1 );

        // Create an input/output stream for this socket.
        inputStream = new SocketInputStream( socketHandle );
        outputStream = new SocketOutputStream( socketHandle );

    } catch( SocketException& ex ) {
        ex.setMark( __FILE__, __LINE__);
        try{ close(); } catch( lang::Exception& cx){ /* Absorb */ }
        throw ex;
    } catch( ... ) {
        try{ close(); } catch( lang::Exception& cx){ /* Absorb */ }
        throw SocketException(
            __FILE__, __LINE__,
            "TcpSocket::connect() - caught unknown exception" );
    }
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::close() throw( lang::Exception )
{
    // Destroy the input stream.
    if( inputStream != NULL ){
        delete inputStream;
        inputStream = NULL;
    }

    // Destroy the output stream.
    if( outputStream != NULL ){
        delete outputStream;
        outputStream = NULL;
    }

    // When connected we first shutdown, which breaks our reads and writes
    // then we close to free APR resources.
    if( isConnected() ) {
        apr_socket_shutdown( socketHandle, APR_SHUTDOWN_READWRITE );
        apr_socket_close( socketHandle );
        socketHandle = INVALID_SOCKET_HANDLE;
    }

    // Destroy the APR Pool
    if( apr_pool != NULL ) {
        apr_pool_destroy( apr_pool );
        apr_pool = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getSoLinger() const throw( SocketException ){

   try{
        int value = 0;
        checkResult( apr_socket_opt_get( socketHandle, APR_SO_LINGER, &value ) );
        return value;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setSoLinger( int dolinger ) throw( SocketException ){

    try{
        checkResult( apr_socket_opt_set( socketHandle, APR_SO_LINGER, dolinger ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool TcpSocket::getKeepAlive() const throw( SocketException ){

    try{
        int value = 0;
        checkResult( apr_socket_opt_get( socketHandle, APR_SO_KEEPALIVE, &value ) );
        return value != 0;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setKeepAlive( const bool keepAlive ) throw( SocketException ){

    try{
        int value = keepAlive ? 1 : 0;
        checkResult( apr_socket_opt_set( socketHandle, APR_SO_KEEPALIVE, value ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getReceiveBufferSize() const throw( SocketException ){

    try{
        int value;
        checkResult( apr_socket_opt_get( socketHandle, APR_SO_RCVBUF, &value ) );
        return value;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setReceiveBufferSize( int size ) throw( SocketException ){

    try{
        checkResult( apr_socket_opt_set( socketHandle, APR_SO_RCVBUF, size ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool TcpSocket::getReuseAddress() const throw( SocketException ){

    try{
        int value;
        checkResult( apr_socket_opt_get( socketHandle, APR_SO_REUSEADDR, &value ) );
        return value != 0;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setReuseAddress( bool reuse ) throw( SocketException ){

    try{
        int value = reuse ? 1 : 0;
        checkResult( apr_socket_opt_set( socketHandle, APR_SO_REUSEADDR, value ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getSendBufferSize() const throw( SocketException ){

    try{
        int value;
        checkResult( apr_socket_opt_get( socketHandle, APR_SO_SNDBUF, &value ) );
        return value;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setSendBufferSize( int size ) throw( SocketException ){

    try{
        checkResult( apr_socket_opt_set( socketHandle, APR_SO_SNDBUF, size ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setSoTimeout ( const int millisecs ) throw ( SocketException )
{
    try{
        // Time is in microseconds so multiply by 1000.
        checkResult( apr_socket_timeout_set( socketHandle, millisecs * 1000 ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
int TcpSocket::getSoTimeout() const throw( SocketException )
{
    try{
        // Time is in microseconds so divide by 1000.
        apr_interval_time_t value = 0;
        checkResult( apr_socket_timeout_get( socketHandle, &value ) );
        return value / 1000;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
bool TcpSocket::getTcpNoDelay() const throw ( lang::Exception ) {

    try{
        int value;
        checkResult( apr_socket_opt_get( socketHandle, APR_TCP_NODELAY, &value ) );
        return value != 0;
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::setTcpNoDelay( bool value ) throw ( lang::Exception ) {

    try{
        int ivalue = value ? 1 : 0;
        checkResult( apr_socket_opt_set( socketHandle, APR_TCP_NODELAY, ivalue ) );
    }
    DECAF_CATCH_RETHROW( SocketException )
    DECAF_CATCHALL_THROW( SocketException )
}

////////////////////////////////////////////////////////////////////////////////
void TcpSocket::checkResult( apr_status_t value ) const throw (SocketException) {

    if( value != APR_SUCCESS ){
        throw SocketException(
            __FILE__, __LINE__,
            SocketError::getErrorString().c_str() );
    }
}
