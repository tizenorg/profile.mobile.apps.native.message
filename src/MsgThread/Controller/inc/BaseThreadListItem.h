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
#include "ContactPersonAddress.h"
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

            void updateThumbnailAndName(const MsgAddressList &addressList, bool decorateName);
            void updateThumbnailAndName(const MsgThreadItem &threadItem, bool decorateName);
            void updateName(const MsgAddress &address, int addressesCount, bool decorateName);
            void updateName(const ContactPersonAddress &address, int addressesCount, bool decorateName);
            void updateName(const std::string &address, int addressesCount, bool decorateName);
            void updateTime(time_t time);
            void updateTime();
            void updateMessage(const MsgThreadItem &threadItem);
            void updateMessage(const std::string &msg);
            time_t getRawTime() const;

        protected:
            // ThreadListViewItem:
            virtual std::string getName();
            virtual std::string getMessage();
            virtual std::string getTime();
            virtual Evas_Object *getThumbnail();

            App &m_App;
            ThumbnailMaker::ThumbId m_ThumbId;
            std::string m_Name;
            std::string m_Message;
            std::string m_Time;
            time_t m_RawTime;
    };
}

#endif // BaseThreadListItem_h_
