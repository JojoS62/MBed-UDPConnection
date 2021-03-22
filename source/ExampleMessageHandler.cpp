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

#include "UDPConnection.h"
#include "ExampleMessageHandler.h"

ExampleMessageHandler::ExampleMessageHandler(UDPConnection* udpConnection) : 
    MessageHandler(udpConnection),
    _threadCyclicSend(osPriorityNormal, 4*1024)
{
    _doSend = false;
}



bool ExampleMessageHandler::dispatch(udp_message_t* message)
{
    bool handled = true;
    _remote = message->remoteAddress;

    printf("ExampleMessageHandler %d bytes '%s'\n", message->size, message->datagram);

    // start cyclic sender
    _doSend = true;

    // do your message handling here

    return handled;
}

void ExampleMessageHandler::start() 
{
    _threadStatus = _threadCyclicSend.start(callback(this, &ExampleMessageHandler::threadFnSend));
}
    


void ExampleMessageHandler::threadFnSend() 
{
    char buffer[] = "cyclic send message";

    while (1) {
        if (_doSend) {
            _udpConnection->sendto(_remote, buffer, sizeof(buffer));
        }
        
        ThisThread::sleep_for(5s);
    }
}