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
#include <vector>
#include <memory>

namespace Msg
{
    class MsgThreadItem;
    typedef std::shared_ptr<MsgThreadItem> BaseMsgThreadItemRef;
    typedef std::vector<BaseMsgThreadItemRef> MsgThreadList;

    class MsgThreadItem
    {
        public:
            MsgThreadItem(ThreadId threadId);
            virtual ~MsgThreadItem();

            virtual void update();

            ThreadId getId() const;
            const std::string &getName() const;
            const std::string &getLastMessage() const;
            std::string getIcon() const;
            int getTime() const;
            int getStatus() const;
            int getUnreadCount() const;

            virtual MsgConversationList getConversationList() const = 0;

        protected:
            ThreadId m_Id;
            std::string m_Name;
            std::string m_LastMessage;
            int m_Time;
    };
}

#endif /* __IMSG_THREAD_ITEM_H__ */
