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
    /**
     * @brief Manages sending(receiving) messages activity.
     */
    class MsgTransport
    {
        public:
            /**
             * @brief Possible results message sending could cause to.
             */
            enum SendResult
            {
                SendSuccess           = 0,      /**< Sending successful.*/
                SendFail              = -1,     /**< Sending failure.*/
                SendNullPointer       = -2,     /**< Invalid parameter passed into sendMessage().*/
                SendNoSIM             = -3,     /**< No SIM was found.*/
                SendMemoryFull        = -4,     /**< No free space available for message-storage.*/
                SendDPMRestricted     = -5,     /**< Sending message is restricted by DPM.*/
            };

        public:
            MsgTransport();
            virtual ~MsgTransport();

            /**
             * @brief Message-service specific implementation of sending message of any type(sms or mms).
             * @param[in, out] msg a message to be sent.
             * @param[out] threadId an id of thread the sent message belongs to.
             * @return result-code.
             */
            virtual SendResult sendMessage(Message &msg, ThreadId *threadId = nullptr) = 0;

            /**
             * @brief General(for calling by client) implementation of sending message of any type(sms or mms).
             * @param[in, out] msg a message to be sent.
             * @param[out] threadId an id of thread the sent message belongs to.
             * @return result-code.
             */
            SendResult sendMessage(MessageRef &msg, ThreadId *threadId = nullptr);

            /**
             * @brief Sending MMS.
             * @param[in, out] msg a message to be sent.
             * @param[out] threadId an id of thread the sent message belongs to.
             * @return result-code.
             */
            SendResult sendMessage(MessageMmsRef &msg, ThreadId *threadId = nullptr);

            /**
             * @brief Sending SMS.
             * @param[in, out] msg a message to be sent.
             * @param[out] threadId an id of thread the sent message belongs to.
             * @return result-code.
             */
            SendResult sendMessage(MessageSMSRef &msg, ThreadId *threadId = nullptr);

            /**
             * @brief Manually downloads MMS by demand.
             * @param[in] msgId id of message to be downloaded.
             */
            virtual void retrieveMessage(MsgId msgId) = 0;
    };
}

#endif /* _BASE_MSG_TRANSPORT_H__ */
