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

            enum Status
            {
                MS_Not_Send = 0,
                MS_Sending,
                MS_Send_Success,
                MS_Send_Fail,
                MS_Deliver_Success,
                MS_Deliver_Fail,
                MS_Received,
                MS_Req_Cancelled,
                MS_Retrieving,
                MS_Retrieve_Success,
                MS_Retrieve_Fail,
                MS_Send_Timeout,
                MS_Send_Fail_Mandatory_Info_Missing,
                MS_Send_Fail_Temporary,
                MS_Send_Fail_By_Mo_Control_With_Mod,
                MS_Send_Fail_By_Mo_Control_Not_Allowed,
                MS_Deliver_Pending,
                MS_Deliver_Expired,
                MS_Send_Pending,
            #ifdef FEATURE_SMS_CDMA
                MS_Send_Fail_Unknown_Subscriber,
                MS_Send_Fail_Ms_Disabled,
                MS_Send_Fail_Network_Not_Ready,
                MS_Retrieve_Pending,
            #endif
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

             virtual std::string getSubject() const = 0;
             virtual void setSubject(const std::string &text) = 0;
    };
}

#endif /* __MESSAGE_H__ */
