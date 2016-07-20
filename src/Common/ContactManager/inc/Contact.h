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

#ifndef __Contact_h__
#define __Contact_h__

#include "ContactRecord.h"

#include <contacts.h>
#include <string>
#include <memory>

namespace Msg
{
    class Contact;
    typedef std::shared_ptr<Contact> ContactRef;

    class Contact
        : public ContactRecord
    {
        public:
            Contact(bool release, contacts_record_h record = nullptr);

            std::string getDispName() const;
            std::string getPhoneNumber() const;
            std::string getEmail() const;
            std::string getThumb() const;
            std::string getAddress() const;
    };

    inline Contact::Contact(bool release, contacts_record_h record)
        : ContactRecord(release, record)
    {
    }

    inline std::string Contact::getDispName() const
    {
        return getStr(_contacts_contact.display_name);
    }

    inline std::string Contact::getPhoneNumber() const
    {
        std::string res;
        int count = getChildCount(_contacts_contact.number);
        if(count > 0)
            res = getStr(getChildP(_contacts_contact.number, 0), _contacts_number.number);
        return res;
    }

    inline std::string Contact::getEmail() const
    {
        std::string res;
        int count = getChildCount(_contacts_contact.email);
        if(count > 0)
            res = getStr(getChildP(_contacts_contact.number, 0), _contacts_email.email);
        return res;
    }

    inline std::string Contact::getThumb() const
    {
        return getStr(_contacts_contact.image_thumbnail_path);
    }

    inline std::string Contact::getAddress() const
    {
        std::string address = getPhoneNumber();
        if(address.empty())
            address = getEmail();
        return address;
    }
}

#endif /* __Contact_h__ */
