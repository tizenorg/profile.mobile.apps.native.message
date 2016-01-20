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
#ifndef __IMSG_THREAD_ITEM_H__
#define __IMSG_THREAD_ITEM_H__

#include "MsgConversationItem.h"
#include "MsgTypes.h"

#include <memory>
#include <time.h>

namespace Msg
{
    class MsgThreadItem;
    typedef std::shared_ptr<MsgThreadItem> MsgThreadItemRef;
    typedef std::shared_ptr<MsgList<MsgThreadItem>> MsgThreadListRef;

    class MsgThreadItem
    {
        public:
            virtual ~MsgThreadItem();

            virtual ThreadId getId() const = 0;
            virtual std::string getName() const = 0;
            virtual std::string getLastMessage() const = 0;
            virtual time_t getTime() const = 0;
            virtual bool hasDraftMessage() const = 0;
            virtual bool hasFailedMessage() const = 0;
            virtual int getUnreadCount() const = 0;
    };
}

#endif /* __IMSG_THREAD_ITEM_H__ */
