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
#ifndef _MESSAGE_SMS_H__
#define _MESSAGE_SMS_H__

#include "Message.h"
#include "MsgList.h"

#include <memory>

namespace Msg
{
    class MessageSMS;
    typedef std::shared_ptr<MessageSMS> MessageSMSRef;
    typedef MsgList<MessageSMS> MessageSMSList;
    typedef std::shared_ptr<MsgList<MessageSMS>> MessageSMSListRef;

    class MessageSMS:
        public virtual Message
    {
        public:
            MessageSMS();
            virtual ~MessageSMS();

            virtual std::string getText() const = 0;
            virtual void setText(const std::string &text) = 0;
    };
}

#endif /* _MESSAGE_SMS_H__ */
