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

#ifndef __MSG_THREAD_ITEM_H__
#define __MSG_THREAD_ITEM_H__

#include "MsgThreadItem.h"
#include "MsgListHandlePrivate.h"
#include "MsgStructListPrivate.h"
#include "MsgStructPrivate.h"

namespace Msg
{
    class MsgThreadItemPrivate
        : public MsgStructPrivate
        , public MsgThreadItem
    {
        public:
            MsgThreadItemPrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgThreadItemPrivate();

            virtual ThreadId getId() const;
            virtual std::string getName() const;
            virtual std::string getLastMessage() const;
            virtual time_t getTime() const;
            virtual bool hasDraftMessage() const;
            virtual bool hasFailedMessage() const;
            virtual int getUnreadCount() const;
    };

    typedef class MsgListHandlePrivate<MsgThreadItemPrivate, MsgThreadItem> MsgThreadListHandlePrivate;
    typedef class MsgStructListPrivate<MsgThreadItemPrivate, MsgThreadItem> MsgThreadStructListPrivate;
}

#endif /* __MSG_THREAD_ITEM_H__ */
