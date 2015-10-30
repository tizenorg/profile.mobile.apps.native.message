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

#include <msg.h>
#include <msg_storage.h>
#include <msg_transport.h>
#include <messages.h>

#include "MsgStorage.h"

namespace Msg
{
    class MsgStoragePrivate
        : public MsgStorage
    {
        public:
            MsgStoragePrivate(msg_handle_t serviceHandle);
            virtual ~MsgStoragePrivate();

            virtual MsgThreadList getThreadList() const;
            virtual MessageRefList getSimMsgList() const;
            virtual int deleteThread(ThreadId id);
            virtual AddressList getAddressListByThreadId(ThreadId id);
            virtual BaseMsgThreadItemRef getThread(ThreadId id) const;
            virtual ThreadId getThreadId(const AddressList &addressList) const;
            virtual MessageRef createMessage(const MsgDataContainer & dataContainer);

        private:
            static void msg_storage_change_cb(msg_handle_t handle, msg_storage_change_type_t storageChangeType, msg_id_list_s *pMsgIdList, void *user_param);
            void onStorageChange();

        private:
            void initThreadList(MsgThreadList &list) const;
            void initSimMsgList(MessageRefList &list) const;

        private:
            msg_handle_t m_ServiceHandleImpl;
    };
}

#endif /* __MSG_STORAGE_PRIVATE_H__ */
