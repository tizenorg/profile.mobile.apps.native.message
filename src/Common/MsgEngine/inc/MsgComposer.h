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

#ifndef __MSG_COMPOSER_H__
#define __MSG_COMPOSER_H__

#include "MessageSMS.h"
#include "MessageMms.h"

namespace Msg
{
    class MsgComposer
    {
        public:
            virtual ~MsgComposer() {};

            virtual MessageSMSRef createSms() = 0;
            virtual MessageMmsRef createMms() = 0;

            MessageRef createMessage(Message::Type type);
    };

    inline MessageRef MsgComposer::createMessage(Message::Type type)
    {
        MessageRef msg;

        switch(type)
        {
            case Message::MT_SMS:
                msg = createSms();
            break;

            case Message::MT_MMS:
                msg = createMms();
            break;

            case Message::MT_Unknown:
            default:
            break;
        }

        return msg;
    }
}

#endif /* __MSG_COMPOSER_H__ */
