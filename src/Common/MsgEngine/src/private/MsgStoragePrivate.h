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

#ifndef __MSG_STORAGE_PRIVATE_H__
#define __MSG_STORAGE_PRIVATE_H__

#include "MsgStorage.h"
#include "MessagePrivate.h"

#include <msg.h>
#include <msg_storage_types.h>

namespace Msg
{
    class MsgStoragePrivate
        : public MsgStorage
    {
        public:
            MsgStoragePrivate(msg_handle_t serviceHandle);
            MsgStoragePrivate(MsgStoragePrivate&) = delete;
            MsgStoragePrivate &operator=(MsgStoragePrivate&) = delete;
            virtual ~MsgStoragePrivate();

            // Thread:
            virtual MsgThreadListRef getThreadList();
            virtual MsgThreadItemRef getThread(ThreadId id);
            virtual ThreadId getThreadId(const MsgAddressList &addressList);
            virtual ThreadId getThreadId(const std::list<std::string> &addressList);
            virtual ThreadId getThreadId(MsgId id);
            virtual bool deleteThread(ThreadId id);
            virtual MsgAddressListRef getAddressList(ThreadId id);
            virtual MsgThreadListRef searchThread(const std::string &word);
            virtual void setReadStatus(ThreadId id);
            virtual int getUnreadThreadCount() const;

            // Message:
            virtual MessageSMSListRef getSimMsgList();
            virtual MessageRef getMessage(MsgId id);

            /**
             *@brief        Return id of new message or of updated message
             *@param[out]    msg - message that we need to save
             *@param[in]    updateExisting - true if we need to update our message id DB,
             *                               false if we need to save message with new id
             *@return       MessageId
             */
            virtual MsgId saveMessage(Message &msg, bool updateExisting);
            virtual bool deleteMessage(MsgId id);
            virtual bool deleteMessages(const MsgIdList &idList);
            virtual MessageListRef searchMessage(const std::string &word);
            virtual void setReadStatus(MsgId id, bool status);
            virtual MsgReportListRef getMsgReportList(MsgId msgId);
            virtual bool isReadReportChecked(MsgId msgId);
            virtual bool isDeliverReportChecked(MsgId msgId);

            // Conversation:
            virtual MsgConversationListRef getConversationList(ThreadId id);
            virtual MsgConversationItemRef getConversationItem(MsgId id);

        private:
            typedef void (IMsgStorageListener::*ListenerMethod)(const MsgIdList &);

            void notifyListeners(const MsgIdList &idList, ListenerMethod method);

            static void msg_storage_change_cb(msg_handle_t handle, msg_storage_change_type_t storageChangeType, msg_id_list_s *pMsgIdList, void *user_param);

            MessageSMS *createSms();

        private:
            msg_handle_t m_ServiceHandle;
    };
}

#endif /* __MSG_STORAGE_PRIVATE_H__ */
