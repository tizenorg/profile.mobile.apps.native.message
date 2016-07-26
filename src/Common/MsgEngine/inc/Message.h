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

    /**
     * @brief An abstraction that represents message-entity stored in message-service.
     */
    class Message
    {
        public:
            /**
             * @brief Each message can be sent or received, and this enum is used to distinguish the kind of every message.
             */
            enum Direction
            {
                MD_Sent = 0,    /**< Outgoing message.*/
                MD_Received     /**< Incoming message.*/
            };

            /**
             * @brief A type of every message.
             */
            enum Type
            {
                MT_Unknown,     /**< Unknown type of message. If you got it something bad has happened.*/
                MT_SMS,         /**< Text message - sms.*/
                MT_MMS,         /**< Multimedia message - mms.*/
                MT_MMS_Noti     /**< Depending on message-settings mms can be downloaded by demand. This type of message is aimed to notify user about new incoming mms is available to download.*/
            };

            /**
             * @brief Status of every message.
             */
            enum NetworkStatus
            {
                NS_Unknown = -1,    /**< Attempt to get message status has failed.*/
                NS_Not_Send,        /**< Message is a draft for now.*/
                NS_Sending,         /**< message is in progress of sending.*/
                NS_Send_Success,    /**< Message was successfully sent.*/
                NS_Send_Fail,       /**< Message was failed to sent.*/
                NS_Deliver_Success, /**< Status of successfully sent delivery report.*/
                NS_Deliver_Fail,    /**< Status of delivery report sending failure.*/
                NS_Received,        /**< Incoming message.*/
                NS_Req_Cancelled,   /**< Message network status request canceled.*/
                NS_Retrieving,      /**< Message is in progress of retrieving. */
                NS_Retrieve_Success,/**< Message was successfully retrieved.*/
                NS_Retrieve_Fail,   /**< Message was failed to retrieve.*/
                NS_Deliver_Pending, /**< Message delivery is pending.*/
                NS_Deliver_Expired, /**< Message delivery is expired.*/
                NS_Send_Pending     /**< Message send is pending. */
            };

            /**
             * @brief Defines a storage for messages.
             */
            enum MessageStorageType
            {
                MS_Unknown,         /**< Unknown storage.*/
                MS_Phone,           /**< Phone's internal storage used for regular messages.*/
                MS_Sim              /**< Sim-card storage used for messages on SIM.*/
            };

        public:
            virtual ~Message();

            /**
             * @brief Gets type of message.
             * @return message type.
             */
            virtual Type getType() const = 0;

            /**
             * @brief Gets id of message.
             * @return message id.
             */
            virtual MsgId getId() const = 0;

            /**
             * @brief Gets id of thread this message belongs to.
             * @return thread id.
             */
            virtual ThreadId getThreadId() const = 0;

            /**
             * @brief Gets time when message was send\received.
             * @return time.
             */
            virtual time_t getTime() const = 0;

            /**
             * @brief Gets list of addresses this message should be delivered to.
             * @return recipients
             */
            virtual const MsgAddressList &getAddressList() const = 0;

            /**
             * @brief Adds one more recipient address into message.
             * @return address added.
             */
            virtual MsgAddress &addAddress() = 0;

            /**
             * @brief Adds multiple recipient addresses into message.
             */
            virtual void addAddresses(const MsgAddressList &list) = 0;

            /**
             * @brief Gets message text.
             * @return text.
             */
            virtual std::string getText() const = 0;

            /**
             * @brief Sets text into message
             * @param[in] text message text.
             */
            virtual void setText(const std::string &text) = 0;

            /**
             * @brief Figures out whether message is outgoing or incoming.
             * @return direction(in\out).
             */
            virtual Message::Direction getDirection() const = 0;

            /**
             * @brief Gets message network status.
             * @return network status.
             */
            virtual NetworkStatus getNetworkStatus() const = 0;

            /**
             * @brief Returns message size,
             * @return size.
             */
            virtual int getSize() const = 0;

            /**
             * @brief Gets message's subject if there's any.
             * @return a subject string.
             */
            virtual std::string getSubject() const = 0;

            /**
             * @brief Adds subject-string into a message or replacing old one if message the subject.
             * @param[in] text a new subject
             */
            virtual void setSubject(const std::string &text) = 0;

            /**
             * @brief Changes message's storage type.
             * @param[in] msgStorage new storage type.
             */
            virtual void setMessageStorageType(Message::MessageStorageType msgStorage) = 0;

            /**
             * @brief Gets type of storage that contains this message.
             */
            virtual Message::MessageStorageType getMessageStorageType() const = 0;

            /**
             * @brief Checks whether this message is mms or not.
             * @return true in case of mms, false otherwise.
             */
            virtual bool isMms() const = 0;
    };
}

#endif /* __MESSAGE_H__ */
