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

#ifndef ThreadSearchListItem_h_
#define ThreadSearchListItem_h_

#include "ThreadListViewItem.h"
#include "ThumbnailMaker.h"
#include "App.h"
#include "Message.h"
#include "MsgTypes.h"

namespace Msg
{
    class ThreadSearchListItem
        : public ThreadListViewItem
    {
        public:
            ThreadSearchListItem(const Message &msg, App &app, const std::string &searchWord);
            virtual ~ThreadSearchListItem();

            MsgId getMsgId() const;

        private:
            void updateModel(const Message &msg, const std::string &searchWord);
            void updateThumbnail(const Message &msg);

            // ThreadListViewItem:
            virtual std::string getName();
            virtual std::string getMessage();
            virtual std::string getTime();
            virtual Evas_Object *getThumbnail();

            Evas_Object *makeUnreadBadge(int unreadCount);

        private:
            MsgId m_MsgId;
            App &m_App;
            ThumbnailMaker::Type m_ThumbType;
            std::string m_ThumbPath;
            std::string m_Name;
            std::string m_Message;
            std::string m_Time;
    };
}

#endif // ThreadSearchListItem_h_
