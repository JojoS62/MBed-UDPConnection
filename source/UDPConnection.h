/* 
 * Copyright (c) 2021 J.Stratmann
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "mbed.h"
#include <list>
#include "EthernetInterface.h"
#include "MessageHandler.h"
#include "UDPMessage.h"

class UDPConnection {
public:
    /**
     * @brief Construct a new UDPConnection object
     * 
     * @param net           existing EthernetInterface
     * @param localPort     listening port. 0 : not bound, socket is not listening
     * @param remote        IP address and port to connect to 
     */
    UDPConnection(EthernetInterface &net, uint16_t localPort, SocketAddress remote = SocketAddress());

    /**
     * @brief Construct a new UDPConnection object, socket not bound and not listening (localPort = 0)
     * 
     * @param net           existing EthernetInterface
     * @param remote        IP address and port to connect to
     */
    UDPConnection(EthernetInterface &net, SocketAddress remote);

    /**
     * @brief start - start communication thread
     * 
     * @return nsapi_error_t 
     */
    nsapi_error_t start();

    /**
     * @brief sendto - send UDP message to address
     * 
     * @param address           remote address
     * @param data              data to send
     * @param size              size of data to send
     * @return nsapi_error_t 
     */
    nsapi_error_t sendto(const SocketAddress &address, const void *data, nsapi_size_t size);

    /**
     * @brief addMessageHandler - add a MessagHandler which is called after UDP receive
     * 
     * @param msgHandler        MessageHandler for incoming data
     */
    void addMessageHandler(MessageHandler* msgHandler) { _messageHandlers.push_back(msgHandler); };

private:
    EthernetInterface &_net;
    uint16_t _localPort;
    SocketAddress _remote;
    UDPSocket _sock;
    Thread  _receiverThread;
    udp_message_t   _dropBuffer;
    uint16_t        _dropCount;

    typedef std::list<MessageHandler*> MessageHandlers;
    MessageHandlers     _messageHandlers;

    void receiverFunction();
};