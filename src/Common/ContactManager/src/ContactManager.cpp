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

#include "ContactManager.h"
#include "Logger.h"
#include "MsgUtils.h"

#include <algorithm>

 namespace Msg
 {
    const int mapReservSize = 512;

    ContactManager::ContactManager()
    {
        MSG_LOG("");
        m_AddressMap.reserve(mapReservSize);
        int error = contacts_connect();
        if(error != 0)
        {
            MSG_LOG_ERROR(whatError(error));
        }

        contacts_db_add_changed_cb(_contacts_contact._uri, contactChangedCb, this);
        contacts_db_add_changed_cb(_contacts_my_profile._uri, contactChangedCb, this);
        contacts_setting_add_name_display_order_changed_cb(contactDisplayOrderChangedCb, this);
    }

    ContactManager::~ContactManager()
    {
        MSG_LOG("");
        int error = contacts_db_remove_changed_cb(_contacts_contact._uri, contactChangedCb, this);
        if(error != 0)
            MSG_LOG_ERROR(whatError(error));

        error = contacts_disconnect();
        if(error != 0)
            MSG_LOG_ERROR(whatError(error));
    }

    std::string ContactManager::whatError(int error)
    {
        switch(error)
        {
            case CONTACTS_ERROR_NONE:
                return "CONTACTS_ERROR_NONE";
            case CONTACTS_ERROR_OUT_OF_MEMORY:
                return "CONTACTS_ERROR_OUT_OF_MEMORY";
            case CONTACTS_ERROR_INVALID_PARAMETER:
                return "CONTACTS_ERROR_INVALID_PARAMETER";
            case CONTACTS_ERROR_FILE_NO_SPACE:
                return "CONTACTS_ERROR_FILE_NO_SPACE";
            case CONTACTS_ERROR_PERMISSION_DENIED:
                return "CONTACTS_ERROR_PERMISSION_DENIED";
            case CONTACTS_ERROR_NOT_SUPPORTED:
                return "CONTACTS_ERROR_NOT_SUPPORTED";
            case CONTACTS_ERROR_NO_DATA:
                return "CONTACTS_ERROR_NO_DATA";
            case CONTACTS_ERROR_DB_LOCKED:
                return "CONTACTS_ERROR_DB_LOCKED";
            case CONTACTS_ERROR_DB:
                return "CONTACTS_ERROR_DB";
            case CONTACTS_ERROR_IPC_NOT_AVALIABLE:
                return "CONTACTS_ERROR_IPC_NOT_AVALIABLE";
            case CONTACTS_ERROR_IPC:
                return "CONTACTS_ERROR_IPC";
            case CONTACTS_ERROR_SYSTEM:
                return "CONTACTS_ERROR_SYSTEM";
            case CONTACTS_ERROR_INTERNAL:
                return "CONTACTS_ERROR_INTERNAL";
        }

        return "Unknown error";
    }


    template <>
    std::shared_ptr<ContactList<ContactPersonPhoneLog>> ContactManager::search<ContactPersonPhoneLog>(const std::string &keyword)
    {
        contacts_query_h query = nullptr;
        contacts_filter_h filter = nullptr;
        contacts_list_h list = nullptr;

        contacts_query_create(_contacts_person_phone_log._uri, &query);
        contacts_filter_create(_contacts_person_phone_log._uri, &filter);
        contacts_filter_add_str(filter, _contacts_person_phone_log.address, CONTACTS_MATCH_CONTAINS, keyword.c_str());
        contacts_filter_add_operator(filter, CONTACTS_FILTER_OPERATOR_AND);
        contacts_filter_add_int(filter, _contacts_person_phone_log.person_id, CONTACTS_MATCH_NONE, 0);
        contacts_query_set_filter(query, filter);

        /* set projection */
        unsigned int numberProjection[] =
        {
            _contacts_person_phone_log.person_id,
            _contacts_person_phone_log.address,
            _contacts_person_phone_log.display_name
        };

        contacts_query_set_projection(query, numberProjection, sizeof(numberProjection)/sizeof(unsigned int));
        contacts_query_set_distinct(query, true);

        contacts_db_get_records_with_query(query, 0, 0, &list);
        contacts_filter_destroy(filter);
        contacts_query_destroy(query);

        return list ? std::make_shared<ContactList<ContactPersonPhoneLog>>(list) : nullptr;
    }

    template<>
    std::shared_ptr<ContactList<ContactPersonNumber>> ContactManager::search<ContactPersonNumber>(const std::string &keyword)
    {
        contacts_list_h list = nullptr;
        contacts_db_search_records_with_range(ContactPersonNumber::getUri(), keyword.c_str(),
                                              0, 0, CONTACTS_SEARCH_RANGE_NAME | CONTACTS_SEARCH_RANGE_NUMBER, &list);
        return list ? std::make_shared<ContactList<ContactPersonNumber>>(list) : nullptr;
    }

    template<>
    std::shared_ptr<ContactList<ContactPersonEmail>> ContactManager::search<ContactPersonEmail>(const std::string &keyword)
    {
        contacts_list_h list = nullptr;
        contacts_db_search_records_with_range(ContactPersonEmail::getUri(), keyword.c_str(),
                                              0, 0, CONTACTS_SEARCH_RANGE_NAME | CONTACTS_SEARCH_RANGE_EMAIL, &list);
        return list ? std::make_shared<ContactList<ContactPersonEmail>>(list) : nullptr;
    }

    ContactPersonNumberRef ContactManager::getContactPersonNumber(int phoneNumId)
    {
        // TODO: impl cache policy if needed
        contacts_filter_h filter = nullptr;
        contacts_filter_create(_contacts_contact_number._uri, &filter);
        contacts_filter_add_int(filter, _contacts_person_number.number_id, CONTACTS_MATCH_EQUAL, phoneNumId);
        return filter ? getContactPersonNumber(filter) : nullptr;
    }

    ContactPersonNumberRef ContactManager::getContactPersonNumber(const std::string &number)
    {
        contacts_filter_h filter = nullptr;
        contacts_filter_create(_contacts_contact_number._uri, &filter);
        contacts_filter_add_str(filter, _contacts_person_number.number_filter, CONTACTS_MATCH_EXACTLY, number.c_str());
        return filter ? getContactPersonNumber(filter) : nullptr;
    }

    ContactPersonEmailRef ContactManager::getContactPersonEmail(const std::string &email)
    {
        contacts_filter_h filter = nullptr;
        contacts_filter_create(_contacts_contact_email._uri, &filter);
        contacts_filter_add_str(filter, _contacts_contact_email.email, CONTACTS_MATCH_EXACTLY, email.c_str());
        return filter ? getContactPersonEmail(filter) : nullptr;
    }

    ContactPersonAddressRef ContactManager::getContactPersonAddress(const std::string &address)
    {
        return getAddress(address);
    }

    ContactOwnerProfileRef ContactManager::getOwnerProfile()
    {
        if(!m_OwnerProfile)
        {
            contacts_list_h list = nullptr;
            contacts_record_h myProfile = nullptr;
            contacts_db_get_all_records(_contacts_my_profile._uri, 0, 1, &list);
            if(list)
            {
                contacts_list_get_current_record_p(list, &myProfile);
                contacts_list_destroy(list, false);
            }
            if(myProfile)
                m_OwnerProfile.reset(new ContactOwnerProfile(true, myProfile));
        }
        return m_OwnerProfile;
    }

    void ContactManager::contactChangedCb(const char *view_uri, void *user_data)
    {
        ContactManager *self = static_cast<ContactManager *>(user_data);
        self->invalidateCache();
        for(auto listener : self->m_Listeners)
        {
            listener->onContactChanged();
        }
    }

    void ContactManager::contactDisplayOrderChangedCb(contacts_name_display_order_e name_display_order, void *user_data)
    {
        ContactManager *self = static_cast<ContactManager *>(user_data);
        self->invalidateCache();
        for(auto listener : self->m_Listeners)
        {
            listener->onContactChanged();
        }
    }

    void ContactManager::addListener(IContactManagerListener &listener)
    {
        auto found = std::find(m_Listeners.begin(), m_Listeners.end(), &listener);
        if(found == m_Listeners.end())
        {
            m_Listeners.push_back(&listener);
        }
    }

    void ContactManager::removeListener(IContactManagerListener &listener)
    {
        auto found = std::find(m_Listeners.begin(), m_Listeners.end(), &listener);
        if(found != m_Listeners.end())
        {
            m_Listeners.erase(found);
        }
    }

    ContactPersonNumberRef ContactManager::getContactPersonNumber(contacts_filter_h filter)
    {
        contacts_query_h query = nullptr;
        contacts_list_h list = nullptr;
        contacts_record_h crValue = nullptr;
        contacts_record_h cResValue = nullptr;

        contacts_query_create(_contacts_person_number._uri, &query);
        contacts_query_set_filter(query, filter);

        unsigned int numberProjection[] =
        {
            _contacts_person_number.person_id,
            _contacts_person_number.number,
            _contacts_person_number.display_name,
            _contacts_person_number.image_thumbnail_path
        };

        int ctRrr = contacts_query_set_projection(query, numberProjection, sizeof(numberProjection) / sizeof(unsigned int));
        if(ctRrr == CONTACTS_ERROR_NONE)
            ctRrr = contacts_db_get_records_with_query(query, 0, 0, &list);

        contacts_filter_destroy(filter);
        contacts_query_destroy(query);

        while(ctRrr == CONTACTS_ERROR_NONE)
        {
            int contactId = 0;

            contacts_list_get_current_record_p(list, &crValue);
            if(crValue)
            {
                contacts_record_get_int(crValue, _contacts_person_number.person_id, &contactId);
                if(contactId > 0)
                    cResValue = crValue;
                else
                    contacts_record_destroy(crValue, true);
            }
            ctRrr = contacts_list_next(list);
        }

        contacts_list_destroy(list, false);
        return cResValue ? std::make_shared<ContactPersonNumber>(true, cResValue) : nullptr;
    }

    ContactPersonEmailRef ContactManager::getContactPersonEmail(contacts_filter_h filter)
    {
        contacts_query_h query = nullptr;
        contacts_list_h list = nullptr;
        contacts_record_h crValue = nullptr;
        contacts_record_h cResValue = nullptr;

        contacts_query_create(_contacts_person_email._uri, &query);
        contacts_query_set_filter(query, filter);

        unsigned int numberProjection[] =
        {
            _contacts_person_email.person_id,
            _contacts_person_email.email,
            _contacts_person_email.display_name,
            _contacts_person_email.image_thumbnail_path
        };

        int ctRrr = contacts_query_set_projection(query, numberProjection, sizeof(numberProjection) / sizeof(unsigned int));
        if(ctRrr == CONTACTS_ERROR_NONE)
            ctRrr = contacts_db_get_records_with_query(query, 0, 0, &list);

        contacts_filter_destroy(filter);
        contacts_query_destroy(query);

        while(ctRrr == CONTACTS_ERROR_NONE)
        {
            int contactId = 0;

            contacts_list_get_current_record_p(list, &crValue);
            if(crValue)
            {
                contacts_record_get_int(crValue, _contacts_person_email.person_id, &contactId);
                if(contactId > 0)
                    cResValue = crValue;
                else
                    contacts_record_destroy(crValue, true);
            }
            ctRrr = contacts_list_next(list);
        }

        contacts_list_destroy(list, false);
        return cResValue ? std::make_shared<ContactPersonEmail>(true, cResValue) : nullptr;
    }

    ContactPersonAddressRef ContactManager::getAddress(const std::string &address)
    {
        auto it = m_AddressMap.find(address);
        if(m_AddressMap.end() == it)
        {
            ContactPersonAddressRef personAddress = MsgUtils::isValidNumber(address) ?
                    std::static_pointer_cast<ContactPersonAddress>(getContactPersonNumber(address)):
                    std::static_pointer_cast<ContactPersonAddress>(getContactPersonEmail(address));
            m_AddressMap[address] = personAddress;

            return personAddress;
        }
        return it->second;
    }

    void ContactManager::invalidateCache()
    {
        // TODO: invalidate cache by uri table
        MSG_LOG("");
        m_AddressMap.clear();
        m_OwnerProfile.reset();
    }
}

