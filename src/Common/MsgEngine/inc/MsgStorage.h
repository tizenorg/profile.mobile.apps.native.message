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

#include "MsgThreadItem.h"
#include "Message.h"
#include "MessageSMS.h"
#include "MsgConversationItem.h"

namespace Msg
{
    class IMsgStorageListener;
    class MsgDataContainer;

    class MsgStorage
    {
        public:
            MsgStorage();
            virtual ~MsgStorage();

            void addListener(IMsgStorageListener &listener);
            void removeListener(IMsgStorageListener &listener);

            // Thread:
            virtual MsgThreadListRef getThreadList() = 0;
            virtual MsgThreadItemRef getThread(ThreadId id) = 0;
            virtual ThreadId getThreadId(const MsgAddressList &addressList) = 0;
            virtual int deleteThread(ThreadId id) = 0;
            virtual MsgAddressListRef getAddressList(ThreadId id) = 0;

            // Message:
            virtual MessageSMSListRef getSimMsgList() = 0;
            virtual MessageRef getMessage(MsgId id) = 0;

            // Conversation:
            virtual MsgConversationListRef getConversationList(ThreadId id) = 0;

        protected:
            typedef std::vector<IMsgStorageListener *> MsgStorageListeners;

        protected:
            MsgStorageListeners m_Listeners;
    };

    class IMsgStorageListener
    {
        public:
            virtual ~IMsgStorageListener() {};

            // TODO: delete this
            virtual void onMsgStorageChange(const MsgIdList &msgIdList) {};

            virtual void onMsgStorageUpdate(const MsgIdList &msgIdList) {};
            virtual void onMsgStorageInsert(const MsgIdList &msgIdList) {};
            virtual void onMsgStorageDelete(const MsgIdList &msgIdList) {};
            virtual void onMsgStorageContact(const MsgIdList &msgIdList) {};
    };
}

#endif /* __MSG_STORAGE_H__ */
