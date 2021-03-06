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

#ifndef _DECAF_NET_URLSTREAMHANDLERFACTORY_H_
#define _DECAF_NET_URLSTREAMHANDLERFACTORY_H_

#include <decaf/util/Config.h>

#include <string>

namespace decaf {
namespace net {

    class URLStreamHandler;

    /**
     * Defines a factory which creates an URLStreamHandler for a specified
     * protocol. It is used by the class URL.
     *
     * @since 1.0
     */
    class URLStreamHandlerFactory {
    public:

        virtual ~URLStreamHandlerFactory();

        /**
         * Creates a new {@code URLStreamHandler} instance for the given {@code
         * protocol}.
         *
         * @param protocol
         *      the protocol for which a handler is needed.
         *
         * @return pointer to the created handler.
         */
        virtual URLStreamHandler* createURLStreamHandler(const std::string& protocol) = 0;

    };

}}

#endif /* _DECAF_NET_URLSTREAMHANDLERFACTORY_H_ */
