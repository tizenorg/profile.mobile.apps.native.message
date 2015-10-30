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
#ifndef _MESSAGE_SMS_PRIVATE_H__
#define _MESSAGE_SMS_PRIVATE_H__

#include "MessagePrivate.h"
#include "MessageSMS.h"

namespace Msg
{
    class MessageSMSPrivate
        : public MessageSMS
        , public MessagePrivate
    {
        public:
            MessageSMSPrivate(msg_handle_t mainHandle);
            MessageSMSPrivate(msg_handle_t mainHandle, msg_struct_t msgStruct);
            virtual ~MessageSMSPrivate();

            virtual Type getType() const;
            virtual void setText(const std::string &text);

            virtual std::string getText() const;

        private:
            void initBaseHandle();
    };
}

#endif /* _MESSAGE_SMS_PRIVATE_H__ */
