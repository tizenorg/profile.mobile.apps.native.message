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

            virtual MsgThreadList getThreadList() const = 0;
            virtual MessageRefList getSimMsgList() const = 0;
            virtual BaseMsgThreadItemRef getThread(ThreadId id) const = 0;
            virtual ThreadId getThreadId(const AddressList &addressList) const = 0;
            virtual int deleteThread(ThreadId id) = 0;

            virtual AddressList getAddressListByThreadId(ThreadId id) = 0;

            virtual MessageRef createMessage(const MsgDataContainer &dataContainer) = 0;

        protected:
            typedef std::vector<IMsgStorageListener *> MsgStorageListeners;

        protected:
            MsgStorageListeners m_Listeners;
    };

    class IMsgStorageListener
    {
        public:
            virtual ~IMsgStorageListener() {};
            virtual void onMsgStorageChange() {};
    };
}

#endif /* __MSG_STORAGE_H__ */
