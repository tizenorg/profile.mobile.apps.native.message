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

#include <string>
#include <memory>

#include "MsgTypes.h"
#include "MsgAddress.h"
#include "MsgList.h"

namespace Msg
{
    class Message;
    typedef std::shared_ptr<Message> MessageRef;
    typedef MsgList<Message> MessageList;
    typedef std::shared_ptr<MsgList<Message>> MessageListRef;

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
                MT_MMS
            };

            enum NetworkStatus
            {
                NS_Unknown = -1,
                NS_Not_Send,
                NS_Sending,
                NS_Send_Success,
                NS_Send_Fail,
                NS_Deliver_Success,
                NS_Deliver_Fail,
                NS_Received,
                NS_Req_Cancelled,
                NS_Retrieving,
                NS_Retrieve_Success,
                NS_Retrieve_Fail,
                NS_Deliver_Pending,
                NS_Deliver_Expired,
                NS_Send_Pending
            };

        public:
            virtual ~Message();

            virtual Type getType() const = 0;
            virtual MsgId getId() const = 0;
            virtual ThreadId getThreadId() const = 0;
            virtual time_t getTime() const = 0;
            virtual const MsgAddressList &getAddressList() const = 0;
            virtual MsgAddress &addAddress() = 0;
            virtual void addAddresses(const MsgAddressList &list) = 0;
            virtual std::string getText() const = 0;
            virtual void setText(const std::string &text) = 0;
            virtual NetworkStatus getNetworkStatus() const = 0;
            virtual std::string getSubject() const = 0;
            virtual void setSubject(const std::string &text) = 0;
    };
}

#endif /* __MESSAGE_H__ */
