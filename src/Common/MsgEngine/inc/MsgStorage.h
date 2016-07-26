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

#ifndef __MSG_STORAGE_H__
#define __MSG_STORAGE_H__

#include <messages.h>
#include <memory>
#include <vector>
#include <list>

#include "MsgThreadItem.h"
#include "Message.h"
#include "MessageSMS.h"
#include "MsgConversationItem.h"
#include "MsgReport.h"

namespace Msg
{
    class IMsgStorageListener;
    class MsgDataContainer;

    /**
     * @brief Instance responsible for interaction with message-service internal message-storage.
     */
    class MsgStorage
    {
        public:
            MsgStorage();
            virtual ~MsgStorage();

            /**
             * @brief Adds a subscriber to be notified about message-storage events.
             * @param[in] listener object listening message-storage events.
             */
            void addListener(IMsgStorageListener &listener);

            /**
             * @brief Removes a subscriber if it was added before. If no such subscriber was added nothing happens.
             * @param[in] listener a subscriber to be removed.
             */
            void removeListener(IMsgStorageListener &listener);

            // Thread:
            /**
             * @brief Gets list of threads existing in message-storage.
             * @return list of threads
             */
            virtual MsgThreadListRef getThreadList() = 0;

            /**
             * @brief Gets thread by a specified id.
             * @param[in] id a thread-id
             * @return thread with id specified
             */
            virtual MsgThreadItemRef getThread(ThreadId id) = 0;

            /**
             * @brief Gets thread by a specified address-list.
             * @param[in] addressList a list of recipient-addresses unique for each thread
             * @return thread with id specified
             */
            virtual ThreadId getThreadId(const MsgAddressList &addressList) = 0;

            /**
             * @brief Gets thread by a specified address-list.
             * @param[in] list of recipient-addresses unique for each thread packed packed in std::list.
             * @return thread with id specified
             */
            virtual ThreadId getThreadId(const std::list<std::string> &addressList) = 0;

            /**
             * @brief Gets thread by a specified message-id.
             * @param[in] id unique message-id this thread contains.
             * @return thread with id specified
             */
            virtual ThreadId getThreadId(MsgId id) = 0;

            /**
             * @brief Deletes thread with specified id.
             * @param[in] id id of thread to be removed.
             * @return true in case of success, otherwise false.
             */
            virtual bool deleteThread(ThreadId id) = 0;

            /**
             * @brief Gets address-list of a thread with specified id.
             * @param[in] id id of thread that contains address list to be removed.
             * @return list of addresses for specified thread.
             */
            virtual MsgAddressListRef getAddressList(ThreadId id) = 0;

            /**
             * @brief Performs a search by specified keyword.
             * @param[in] word a search keyword.
             * @return list of threads that contain messages with text matched with a keyword.
             */
            virtual MsgThreadListRef searchThread(const std::string &word) = 0;

            /**
             * @brief Marks thread with specified id as read.
             * @param[in] id id of thread to mark as read.
             */
            virtual void setReadStatus(ThreadId id) = 0;

            /**
             * @brief Gets number of threads with unread messages.
             * @return number of unread threads.
             */
            virtual int getUnreadThreadCount() const = 0;

            /**
             * @brief Checks whether thread with specified id has email in it's address-list.
             * @param[in] id an id of thread.
             * @return true if thread has email in it's address-list, otharwise false.
             */
            bool hasEmail(ThreadId id);

            // Message:
            /**
             * @brief Gets a list of messages on SIM.
             * @return list messages on SIM.
             */
            virtual MessageSMSListRef getSimMsgList() = 0;

            /**
             * @brief Gets message by specified message-id.
             * @param[in] id id of message to be found.
             * @return found message if it's exists or empty MessageRef if search fails.
             */
            virtual MessageRef getMessage(MsgId id) = 0;

            /**
             * @brief Commits edited message into message-storage in two optional way:
             * -# save existing message
             * -# create a copy of source message
             * @param[out] msg message to be saved.
             * @param[in] a updateExisting flag that defines a saving strategy. If true the existing message is saved, otherwise a new message-copy is created.
             * @return id of saved message.
             */
            virtual MsgId saveMessage(Message &msg, bool updateExisting) = 0;

            /**
             * @brief Deletes single message with specified id.
             * @param[in] id an id of message to be removed.
             * @return true in case of successful deletion, otherwise false.
             */
            virtual bool deleteMessage(MsgId id) = 0;

            /**
             * @brief Deletes multiple messages with specified ids.
             * @param[in] idList list of message-id to be removed.
             * @return true in case of successful deletion, otherwise false. Also returns false if idList is empty.
             */
            virtual bool deleteMessages(const MsgIdList &idList) = 0;

            /**
             * @brief Performs search of a message based on a keyword.
             * @param[in] word a search keyword.
             * @return list of messages with text matched with a keyword.
             */
            virtual MessageListRef searchMessage(const std::string &word) = 0;

            /**
             * @brief Marks message with specified id as read.
             * @param[in] id id of message to mark as read
             * @param[in] new read-status a status of message. Set @c true if the message is read, otherwise set @c false if the message is not read
             */
            virtual void setReadStatus(MsgId id, bool status) = 0;

            /**
             * @brief Gets list of reports(Delivery/Read) relevand for mwssage with specified id.
             * @param[in] msgId message-id.
             * @return list of reports.
             */
            virtual MsgReportListRef getMsgReportList(MsgId msgId) = 0;

            virtual bool isReadReportChecked(MsgId msgId) = 0; // TODO: remove to MmsMessage
            virtual bool isDeliverReportChecked(MsgId msgId) = 0;  //TODO: remove to MmsMessage

            // Conversation:
            /**
             * @brief Gets conversation-list that belongs to thread with specified id.
             * @param[in] id thread id.
             * @return List of messages from thread.
             */
            virtual MsgConversationListRef getConversationList(ThreadId id) = 0;

            /**
             * @brief Gets an item from conversation-list corresponded to message with specified id.
             * @param[in] id message id.
             * @return Conversation-list item.
             */
            virtual MsgConversationItemRef getConversationItem(MsgId id) = 0;

        protected:
            typedef std::vector<IMsgStorageListener *> MsgStorageListeners;

        protected:
            MsgStorageListeners m_Listeners;
    };


    /**
     * @brief A listener aimed to notify subscribers about message-storage events.
     */
    class IMsgStorageListener
    {
        public:
            virtual ~IMsgStorageListener() {};

            /**
             * @brief Message-storage updates for the given messages.
             * @param[in] msgIdList list of message identifiers relevant to this notification.
             */
            virtual void onMsgStorageUpdate(const MsgIdList &msgIdList) {};

            /**
             * @brief Notification about new messages addition.
             * @param[in] msgIdList list of message identifiers relevant to this notification.
             */
            virtual void onMsgStorageInsert(const MsgIdList &msgIdList) {};

            /**
             * @brief Notification about new messages deletion.
             * @param[in] msgIdList list of message identifiers relevant to this notification.
             */
            virtual void onMsgStorageDelete(const MsgIdList &msgIdList) {};

            /**
             * @brief Notifies that recipient stored in contacts was updated.
             * @param[in] msgIdList list of message identifiers relevant to this notification.
             */
            virtual void onMsgStorageContact(const MsgIdList &msgIdList) {};
    };
}

#endif /* __MSG_STORAGE_H__ */
