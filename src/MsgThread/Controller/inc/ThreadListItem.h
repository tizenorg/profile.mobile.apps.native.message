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

#ifndef ThreadListItem_h_
#define ThreadListItem_h_

#include "BaseThreadListItem.h"
#include "MsgThreadItem.h"
#include "ThumbnailMaker.h"
#include "ContactPersonNumber.h"
#include "ContactManager.h"
#include "App.h"

namespace Msg
{
    class ThreadListItem
        : public BaseThreadListItem
    {
        public:
            ThreadListItem(const MsgThreadItem &threadItem, App &app);
            virtual ~ThreadListItem();

            ThreadId getThreadId() const;
            void updateModel(const MsgThreadItem &threadItem);

        private:
            // ThreadListViewItem:
            virtual std::string getStatus();
            virtual Evas_Object *getIcon();

        private:
            ThreadId m_ThreadId;
            std::string m_Status;
            std::string m_UnreadCount;
    };
}

#endif // ThreadListItem_h_
