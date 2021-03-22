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

DigitalOut  led(LED1);

UDPConnection::UDPConnection(EthernetInterface &net, uint16_t localPort, SocketAddress remote) :
    _net(net),
    _localPort(localPort),
    _remote(remote),
    _dropCount(0)
{
}

UDPConnection::UDPConnection(EthernetInterface &net,  SocketAddress remote) :
    _net(net),
    _localPort(0),
    _remote(remote),
    _dropCount(0)
{
}

nsapi_error_t UDPConnection::start()
{
    volatile nsapi_error_t err;

    // open UDP socket
    err = _sock.open(&_net);
    if (err != NSAPI_ERROR_OK) {
        return err;
    }

    // bind local port
    if (_localPort != 0) {
        err = _sock.bind(_localPort);
        if (err != NSAPI_ERROR_OK) {
            _sock.close();
            return err;
        }
    }
    
    // connect if remote addr is not empty
    if (_remote.get_port() != 0) {
        err = _sock.connect(_remote);
        if (err != NSAPI_ERROR_OK) {
            _sock.close();
            return err;
        }
    }

    _sock.set_blocking(true);
    _receiverThread.start(callback(this, &UDPConnection::receiverFunction));

    // init message handlers
    for(auto msgHandler : _messageHandlers) {
        (*msgHandler).start();
    }

    return err;
}

nsapi_error_t UDPConnection::sendto(const SocketAddress &address, const void *data, nsapi_size_t size)
{
    return _sock.sendto(address, data, size);
}

void UDPConnection::receiverFunction()
{
    volatile nsapi_error_t err = NSAPI_ERROR_OK;
    udp_message_t *message = new udp_message_t;

    while(true) {                                   // read forever
        err = _sock.recvfrom(&message->remoteAddress, &message->datagram, sizeof(message->datagram));
        if (err > 0) {
            message->size = err;
            for(auto msgHandler : _messageHandlers) {
                if ((*msgHandler).dispatch(message)) {
                    break;                              // message was handled
                }
            }
        }
    }
}
