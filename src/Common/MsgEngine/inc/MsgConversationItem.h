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

    /**
     * @brief An interface that provides basic information about conversation-list item.
     */
    class MsgConversationItem
    {
        public:
            virtual ~MsgConversationItem();

            /**
             * @brief Gets id of thread this conversation-item belongs to.
             * @return thread id.
             */
            virtual ThreadId getThreadId() const = 0;

            /**
             * @brief Gets id of message this conversation-item is related with.
             * @return message id.
             */
            virtual MsgId getMsgId() const = 0;

            /**
             * @brief Gets message-text displayed in conversation-item.
             * @return message text.
             */
            virtual std::string getText() const = 0;

            /**
             * @brief Gets subject-string.
             * @brief subject.
             */
            virtual std::string getSubject() const = 0;

            /**
             * @brief Gets a time when message status was updated last time(it could happen after sending\receiving\editing draft/etc).
             * @return time.
             */
            virtual time_t getTime() const = 0;

            /**
             * @brief Gets message direction.
             * @return direction: Outgoing or Incoming
             */
            virtual Message::Direction getDirection() const = 0;

            /**
             * @brief Gets message type.
             * @return message type.
             */
            virtual Message::Type getType() const = 0;

            /**
             * @brief Gets message network status.
             * @return message network status.
             */
            virtual Message::NetworkStatus getNetworkStatus() const = 0;

            /**
             * @brief Checks whether conversation-item is related to draft message or not.
             * @return true if message is draft, false otherwise.
             */
            virtual bool isDraft() const = 0;

            /**
             * @brief Checks whether conversation-item is related to read message or not.
             * @return true if message is read, false otherwise.
             */
            virtual bool isRead() const = 0;

            /**
             * @brief Gets the count of pages message related to this conversation item contains.
             * @return number of pages.
             */
            virtual int getPagesCount() const = 0;

            /**
             * @brief Gets the count of attachments message related to this conversation item contains.
             * @return number of attachments.
             */
            virtual int getAttachCount() const = 0;

            /**
             * @brief Gets media-files list.
             * @return media-files list.
             */
            virtual const MsgConvMediaList &getMediaList() const = 0;
    };
}

#endif /* __MSG_CONVERSATION_ITEM_H__ */
