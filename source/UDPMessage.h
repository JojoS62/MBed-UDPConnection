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
/**
 * @brief udp message with meta information as size and remote address
 * 
 */
typedef struct {
    int             size;
    SocketAddress   remoteAddress;
    char            datagram[1500];     // todo: replace size by udpsize
} udp_message_t;

