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
#ifndef __MSG_CONVERSATION_ITEM_PRIVATE_H__
#define __MSG_CONVERSATION_ITEM_PRIVATE_H__

#include "MsgConversationItem.h"
#include <msg.h>

namespace Msg
{
    class MsgConversationItemPrivate
        : public MsgConversationItem
    {
        public:
            MsgConversationItemPrivate(msg_handle_t serviceHandle, ThreadId threadId, ConvItemId convItemId);
            virtual ~MsgConversationItemPrivate();

            virtual void update();

        private:
            void updateText(msg_struct_t convItemInfo = nullptr);
            void updateTime(msg_struct_t convItemInfo = nullptr);
            void updateDirection(msg_struct_t convItemInfo = nullptr);

        private:
            msg_handle_t m_ServiceHandle;
    };
}





#endif /* __MSG_CONVERSATION_ITEM_PRIVATE_H__ */
