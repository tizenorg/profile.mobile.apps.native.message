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
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <memory>
#include <vector>
#include <string>

#include "MsgTypes.h"

namespace Msg
{
    class Message;
    typedef std::shared_ptr<Message> MessageRef;
    typedef std::vector<MessageRef> MessageRefList;
    typedef std::vector<std::string> AddressList;

    class Message
    {
        public:
            enum Direction
            {
                MD_Sent = 0,
                MD_Received
            };

            enum Type
            {
                MT_Unknown,
                MT_SMS,
                MT_MMS,
                MT_MAX
            };

        public:
            Message();
            virtual ~Message();

            virtual Type getType() const = 0;
            virtual void setAddressList(const AddressList &addressList) = 0;
            virtual ThreadId getThreadId() const = 0;
            virtual std::string getText() const = 0;
            virtual time_t getTime() const = 0;
            virtual std::string getNumber() const = 0;
    };
}



#endif /* __MESSAGE_H__ */
