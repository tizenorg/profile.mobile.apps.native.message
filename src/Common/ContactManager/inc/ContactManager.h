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
#include <memory>
#include <unordered_map>

#include "ContactList.h"
#include "ContactPersonPhoneLog.h"
#include "ContactPersonNumber.h"
#include "ContactPersonEmail.h"
#include "ContactMyProfile.h"
#include "ContactMyProfileNumber.h"
#include "ContactMyProfileEmail.h"

namespace Msg
{
    /*
     *@brief        A listener to be notified after contact-information will be changed in contacts-db
     */
    class IContactManagerListener;
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
            std::shared_ptr<ContactList<T>> search(const std::string &keyword);

            /**
             *@brief        Search by email and phone and return ContactAddress record
             *@param[in]    address - email or number
             *@return       Reference to ContactAddress
             */
            ContactAddressRef getContactAddress(const std::string &address);

            /**
             *@brief        Search for contacts-id, name, phone-number and thumbnail path based on phone-number id
             *@param[in]    phoneId - identifier of phone-number in contacts-database
             *@return       Reference to ContactPersonNumber
             */
            ContactPersonNumberRef getContactPersonNumber(int phoneId);

            /**
             *@brief        Search by email address ContactPersonEmail record
             *@param[in]    email - email address
             *@return       Reference to ContactPersonEmail
             */
            ContactPersonEmailRef getContactPersonEmail(const std::string &email);

            /**
             *@brief        Search by number address ContactMyProfileNumber record
             *@param[in]    number - phone number
             *@return       Reference to ContactMyProfileNumber
             */
            ContactMyProfileNumberRef getContactMyProfileNumber(const std::string &number);

            /**
             *@brief        Search by email address ContactMyProfileEmailRef record
             *@param[in]    email - email address
             *@return       Reference to ContactMyProfileEmailR
             */
            ContactMyProfileEmailRef getContactMyProfileEmail(const std::string &email);

            /**
             *@brief        Return owner's ContactPersonAddress record
             *@return       Reference to ContactPersonAddress
             */
            ContactMyProfileRef getOwnerProfile();

            /**
             *@brief        Add listener on contacts database
             *@param[in]    listener
             */
            void addListener(IContactManagerListener &listener);

            /**
             *@brief        Remove listener
             *@param[in]    listener - listener to be remove
             */
            void removeListener(IContactManagerListener &listener);

            /**
             *@brief        Create content of contact by it`s id
             *@param[in]    personId - contact id
             *@param[in]    myProfile - is app control my_profile data type
             *@return       Content with all info about one contact
             */
            std::string makeVcard(const int personId, bool myProfile);

            /**
             *@brief        Create content of contact by id list
             *@param[in]    personId - contact id
             *@return       Content with all info about contacts
             */
            std::string makeVcard(const std::list<int> &idList);

        private:
            typedef std::unordered_map<std::string, ContactAddressRef> AddressMap;

        private:
            /**
             *@brief        A callback passed to contacts_db_add_changed_cb() to get notifications
                            about contacts modifications from contact service
             *@param[in]    view_uri - The view URI of records whose changes are monitored
             *@param[in]    user_data - the user data passed from the callback registration function
             */
            static void contactChangedCb(const char *view_uri, void *user_data);
            static void contactDisplayOrderChangedCb(contacts_name_display_order_e name_display_order, void *user_data);

            /**
             * @brief Generic search for basic contact attributes based on custom filter passed from above
             * @param[in] filter to be used for db-request. DO NOT destroy filter manually, it will be destroyed insithe this method.
             */
            ContactPersonNumberRef getContactPersonNumber(contacts_filter_h filter);
            ContactPersonEmailRef getContactPersonEmail(contacts_filter_h filter);

            /**
             *@brief        Search by phone number ContactPersonNumber record
             *@param[in]    number - phone number
             *@return       Reference to ContactPersonNumber
             */
            ContactPersonNumberRef getContactPersonNumber(const std::string &number);


            void invalidateCache();
            ContactAddressRef getAddress(const std::string &address);

            std::string createContactContent(contacts_record_h record, bool myProfile);
            std::string createContentForContactList(int personId);

        private:
            std::list<IContactManagerListener *> m_Listeners;
            AddressMap m_AddressMap;
            ContactMyProfileRef m_OwnerProfile;
    };

    class IContactManagerListener
    {
        public:
            virtual ~IContactManagerListener() {}
            /*
             *@brief        A method called when changing of some contact-record has been occurred
             */
            virtual void onContactChanged() {};
    };
}

#endif /* __ContactManager_h__ */
