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

#ifndef BaseThreadListItem_h_
#define BaseThreadListItem_h_

#include "ThreadListViewItem.h"
#include "ThumbnailMaker.h"
#include "MsgThreadItem.h"
#include "MsgAddress.h"
#include "App.h"

#include <time.h>

namespace Msg
{
    class BaseThreadListItem
        : public ThreadListViewItem
    {
        public:
            BaseThreadListItem(App &app);
            virtual ~BaseThreadListItem();

        protected:
            void updateThumbnail(const MsgAddress &addr);
            void updateThumbnail(const MsgAddressList &addressList);
            void updateThumbnail(const MsgThreadItem &threadItem);
            void updateTime(time_t time);

            // ThreadListViewItem:
            virtual std::string getName();
            virtual std::string getMessage();
            virtual std::string getTime();
            virtual Evas_Object *getThumbnail();

            App &m_App;
            ThumbnailMaker::Type m_ThumbType;
            std::string m_ThumbPath;
            std::string m_Name;
            std::string m_Message;
            std::string m_Time;
    };
}

#endif // BaseThreadListItem_h_
