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

#ifndef __MSG_CONVERSATION_ITEM_H__
#define __MSG_CONVERSATION_ITEM_H__

#include "MsgTypes.h"
#include "Message.h"
#include "MsgConvMedia.h"

#include <string>
#include <time.h>
#include <memory>

namespace Msg
{
    class MsgConversationItem;
    typedef std::shared_ptr<MsgConversationItem> MsgConversationItemRef;
    typedef std::shared_ptr<MsgList<MsgConversationItem>> MsgConversationListRef;

    class MsgConversationItem
    {
        public:
            virtual ~MsgConversationItem();

            virtual ThreadId getThreadId() const = 0;
            virtual MsgId getMsgId() const = 0;
            virtual std::string getText() const = 0;
            virtual std::string getSubject() const = 0;
            virtual time_t getTime() const = 0;
            virtual Message::Direction getDirection() const = 0;
            virtual Message::Type getType() const = 0;
            virtual Message::Status getStatus() const = 0;
            virtual bool isDraft() const = 0;
            virtual bool isRead() const = 0;
            virtual int getPagesCount() const = 0;
            virtual int getAttachCount() const = 0;
            virtual std::string getAttachName() const = 0;
            virtual std::string getAudioName() const = 0;
            virtual std::string getImageThumbPath() const = 0;
            virtual std::string getVideoThumbPath() const = 0;
            virtual std::string getFirstMediaPath() const = 0;
            virtual const MsgConvMediaList &getMultipartList() const = 0;
    };
}

#endif /* __MSG_CONVERSATION_ITEM_H__ */
