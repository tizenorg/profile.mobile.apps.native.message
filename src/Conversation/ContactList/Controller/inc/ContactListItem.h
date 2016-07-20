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

#ifndef ContactListItem_h_
#define ContactListItem_h_

#include "ContactListViewItem.h"
#include "ContactPersonPhoneLog.h"
#include "ContactAddress.h"
#include "App.h"

namespace Msg
{
    class ContactListItem
        : public ContactListViewItem
    {
        public:
            ContactListItem(const ContactPersonPhoneLog &rec, App &app, const std::string &searchWord);
            ContactListItem(const ContactAddress &rec, App &app, const std::string &searchWord);
            virtual ~ContactListItem();

            const std::string &getRecipient() const;

        private:
            ContactListItem(const std::string &recipient, App &app);
            virtual std::string getSubText() const;
            virtual std::string getMainText() const;
            Evas_Object *getThumbnail() const;

        private:
            App &m_App;
            std::string m_Recipient;
            ThumbId m_ThumbId;
            std::string m_MainText;
            std::string m_SubText;
    };
}

#endif // ContactListItem_h_
