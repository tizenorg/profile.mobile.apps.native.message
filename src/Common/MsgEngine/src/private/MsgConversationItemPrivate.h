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
#include "MsgStructPrivate.h"
#include "MsgListHandlePrivate.h"
#include "MsgStructListPrivate.h"
#include "MsgConvMediaPrivate.h"

namespace Msg
{
    /**
     * @brief Implementation of MsgConversationItem with msg-service private API.
     */
    class MsgConversationItemPrivate
        : public MsgStructPrivate
        , public MsgConversationItem
    {
        public:
            MsgConversationItemPrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgConversationItemPrivate();

            virtual MsgId getMsgId() const;
            virtual ThreadId getThreadId() const;
            virtual std::string getText() const;
            virtual std::string getSubject() const;
            virtual time_t getTime() const;
            virtual Message::Direction getDirection() const;
            virtual Message::Type getType() const;
            virtual Message::NetworkStatus getNetworkStatus() const;
            virtual bool isDraft() const;
            virtual bool isRead() const;
            virtual int getPagesCount() const;
            virtual int getAttachCount() const;
            virtual const MsgConvMediaListHandlePrivate &getMediaList() const;

        protected:
            mutable MsgConvMediaListHandlePrivate m_MultipartList;
    };

    typedef class MsgListHandlePrivate<MsgConversationItemPrivate, MsgConversationItem> MsgConversationListHandlePrivate;
    typedef class MsgStructListPrivate<MsgConversationItemPrivate, MsgConversationItem> MsgConversationStructListPrivate;
}

#endif /* __MSG_CONVERSATION_ITEM_PRIVATE_H__ */
