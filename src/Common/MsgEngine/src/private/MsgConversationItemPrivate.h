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
            virtual Message::Status getStatus() const;
            virtual bool isDraft() const;
            virtual bool isRead() const;
            virtual int getPagesCount() const;
            virtual int getAttachCount() const;
            virtual std::string getAttachName() const;
            virtual std::string getAudioName() const;
            virtual std::string getImageThumbPath() const;
            virtual std::string getVideoThumbPath() const;
            virtual std::string getFirstMediaPath() const;
            virtual const MsgConvMediaListHandlePrivate &getMultipartList() const;

        protected:
            MsgConvMediaListHandlePrivate m_MultipartList;
    };

    typedef class MsgListHandlePrivate<MsgConversationItemPrivate, MsgConversationItem> MsgConversationListHandlePrivate;
    typedef class MsgStructListPrivate<MsgConversationItemPrivate, MsgConversationItem> MsgConversationStructListPrivate;
}

#endif /* __MSG_CONVERSATION_ITEM_PRIVATE_H__ */
