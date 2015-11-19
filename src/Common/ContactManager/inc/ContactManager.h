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

#ifndef __ContactManager_h__
#define __ContactManager_h__

#include <string>
#include <list>
#include <vector>
#include <contacts.h>

#include "ContactList.h"
#include "ContactPersonPhoneLog.h"
#include "ContactPersonNumber.h"

namespace Msg
{
    class IContactDbChangeListener;

    class ContactManager
    {
        public:
            ContactManager();
            ~ContactManager();
            ContactManager(ContactManager&) = delete;
            ContactManager &operator=(ContactManager&) = delete;

        public:
            static std::string whatError(int error);

            template<typename T>
            ContactList<T> search(const std::string &keyword);
            ContactPersonNumber getContactPersonNumber(const std::string &number) const;

            void addListener(IContactDbChangeListener &listener);
            void removeListener(IContactDbChangeListener &listener);

        private:
            static void contactChangedCb(const char *view_uri, void *user_data);

        private:
            std::vector<IContactDbChangeListener *> m_Listeners;
    };

    class IContactDbChangeListener
    {
        public:
            virtual ~IContactDbChangeListener() {}
            virtual void onContactChanged() {};
    };
}

#endif /* __ContactManager_h__ */
