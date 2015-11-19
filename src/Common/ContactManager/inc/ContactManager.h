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
            /*
             *@brief        function return what kind of error occurred
             *@param[in]    error - number of error
             *@return       kind of error
             */
            static std::string whatError(int error);
            /*
             *@brief        function search data of contacts person
             *@param[in]    keyword - keyword which to search data
             *@return       ContactList
             */
            template<typename T>
            ContactList<T> search(const std::string &keyword);
            /*
             *@brief        function find by number: id, name and thumbnail path
             *@param[in]    number - contact person number
             *@return       ContactPersonNumber
             */
            ContactPersonNumber getContactPersonNumber(const std::string &number) const;
            /*
             *@brief        function add listener on contacts database
             *@param[in]    listener
             */
            void addListener(IContactDbChangeListener &listener);
            /*
             *@brief        function remove listener
             *@param[in]    listener - listener to be remove
             */
            void removeListener(IContactDbChangeListener &listener);

        private:
            /*
             *@brief        The callback function to register when there have been changes in contacts database
             *@param[in]    view_uri - the view URI
             *@param[in]    user_data - the user data passed from the callback registration function
             */
            static void contactChangedCb(const char *view_uri, void *user_data);

        private:
            std::list<IContactDbChangeListener *> m_Listeners;
    };

    class IContactDbChangeListener
    {
        public:
            virtual ~IContactDbChangeListener() {}
            virtual void onContactChanged() {};
    };
}

#endif /* __ContactManager_h__ */
