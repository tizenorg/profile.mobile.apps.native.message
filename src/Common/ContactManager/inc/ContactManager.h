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
    /*
     *@brief        A listener to be notified after contact-information will be changed in contacts-db
     */
    class IContactDbChangeListener;
    /*
     *@brief        Class that work with contacts database.
     *@brief        Search contacts, add listeners, register callback on changes in database.
     */
    class ContactManager
    {
        public:
            ContactManager();
            ~ContactManager();
            ContactManager(ContactManager&) = delete;
            ContactManager &operator=(ContactManager&) = delete;

        public:
            /**
             *@brief        Return what kind of error occurred
             *@param[in]    error - number of error
             *@return       kind of error
             */
            static std::string whatError(int error);
            /**
             * @brief       Looks for contact information based on a given keyword
             * @param       keyword - search keyword
             */
            template<typename T>
            ContactList<T> search(const std::string &keyword);
            /**
             *@brief        search for contacts-id, name, phone-number and thumbnail path based on phone-number
             *@param[in]    number - contact person number
             *@return       ContactPersonNumber
             */
            ContactPersonNumber getContactPersonNumber(const std::string &number) const;

            /**
             *@brief        Search for contacts-id, name, phone-number and thumbnail path based on phone-number id
             *@param[in]    phoneId - identifier of phone-number in contacts-database
             *@return       ContactPersonNumber
             */
            ContactPersonNumber getContactPersonNumber(int phoneId) const;

            /**
             *@brief        Add listener on contacts database
             *@param[in]    listener
             */
            void addListener(IContactDbChangeListener &listener);
            /**
             *@brief        Remove listener
             *@param[in]    listener - listener to be remove
             */
            void removeListener(IContactDbChangeListener &listener);

        private:
            /**
             *@brief        A callback passed to contacts_db_add_changed_cb() to get notifications
                            about contacts modifications from contact service
             *@param[in]    view_uri - The view URI of records whose changes are monitored
             *@param[in]    user_data - the user data passed from the callback registration function
             */
            static void contactChangedCb(const char *view_uri, void *user_data);

            /**
             * @brief Generic search for basic contact attributes based on custom filter passed from above
             * @param[in] filter to be used for db-request. DO NOT destroy filter manually, it will be destroyed insithe this method.
             */
            ContactPersonNumber getContactPersonNumber(contacts_filter_h filter) const;

        private:
            std::list<IContactDbChangeListener *> m_Listeners;
    };

    class IContactDbChangeListener
    {
        public:
            virtual ~IContactDbChangeListener() {}
            /*
             *@brief        A method called when changing of some contact-record has been occurred
             */
            virtual void onContactChanged() {};
    };
}

#endif /* __ContactManager_h__ */
