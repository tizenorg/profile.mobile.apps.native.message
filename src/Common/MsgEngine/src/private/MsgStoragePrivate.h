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

#include <msg.h>
#include <msg_storage_types.h>

namespace Msg
{
    class MsgStoragePrivate
        : public MsgStorage
    {
        public:
            MsgStoragePrivate(msg_handle_t serviceHandle);
            virtual ~MsgStoragePrivate();

            // Thread:
            virtual MsgThreadListRef getThreadList();
            virtual MsgThreadItemRef getThread(ThreadId id);
            virtual ThreadId getThreadId(const MsgAddressList &addressList);
            virtual int deleteThread(ThreadId id);
            virtual MsgAddressListRef getAddressList(ThreadId id);

            // Message:
            virtual MessageRef createMessage(Message::Type type);
            virtual MessageSMSListRef getSimMsgList();

            // Conversation:
            virtual MsgConversationListRef getConversationList(ThreadId id);

        private:
            static void msg_storage_change_cb(msg_handle_t handle, msg_storage_change_type_t storageChangeType, msg_id_list_s *pMsgIdList, void *user_param);
            void onStorageChange();

            MessageSMS *createSms();

        private:
            msg_handle_t m_ServiceHandle;
    };
}

#endif /* __MSG_STORAGE_PRIVATE_H__ */
