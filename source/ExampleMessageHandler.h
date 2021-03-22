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
#include "MessageHandler.h"

class ExampleMessageHandler : public MessageHandler {
public:
    /**
     * @brief Construct a new MTMessageHandler object
     * 
     * @param udpConnection - UDPConnection object which calls the message handler
     */
    ExampleMessageHandler(UDPConnection* udpConnection);

    /**
     * @brief dispatch - dispatch an incoming message
     * 
     * @param message 
     * @return true 
     * @return false 
     */
    bool dispatch(udp_message_t* message) override;

    /**
     * @brief start - perform action after connection start
     * 
     */
    void start() override;
    
private:
    Thread _threadCyclicSend;
    osStatus _threadStatus;
    SocketAddress _remote;
    void threadFnSend();
    bool _doSend;
};