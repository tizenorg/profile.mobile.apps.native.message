/*
 * Copyright (c) 2009-2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MSG_TRANSPORT_PRIVATE_H__
#define _MSG_TRANSPORT_PRIVATE_H__

#include "MsgTransport.h"

#include <msg.h>
#include <msg_storage.h>
#include <msg_transport.h>

namespace Msg
{
    class MsgTransportPrivate
        : public MsgTransport
    {
        public:
            MsgTransportPrivate(msg_handle_t serviceHandle);
            virtual ~MsgTransportPrivate();

            virtual ComposerReturnType sendMessage(const Message &msg, ThreadId *threadId);

        private:
            msg_handle_t m_ServiceHandle;
    };
}

#endif /* _MSG_TRANSPORT_PRIVATE_H__ */
