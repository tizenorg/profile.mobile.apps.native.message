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
 *
 */

#ifndef _BASE_MSG_TRANSPORT_H__
#define _BASE_MSG_TRANSPORT_H__

#include "Message.h"
#include "MessageMms.h"
#include "MessageSMS.h"

namespace Msg
{
    class MsgTransport
    {
        public:
            enum SendResult
            {
                SendSuccess           = 0,
                SendFail              = -1,
                SendNullPointer       = -2,
                SendNoSIM             = -3,
                SendMemoryFull        = -4,
            };

        public:
            MsgTransport();
            virtual ~MsgTransport();

            virtual SendResult sendMessage(Message &msg, ThreadId *threadId = nullptr) = 0;
            SendResult sendMessage(MessageRef &msg, ThreadId *threadId = nullptr);
            SendResult sendMessage(MessageMmsRef &msg, ThreadId *threadId = nullptr);
            SendResult sendMessage(MessageSMSRef &msg, ThreadId *threadId = nullptr);
    };
}

#endif /* _BASE_MSG_TRANSPORT_H__ */
