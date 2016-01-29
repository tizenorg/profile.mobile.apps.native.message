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

#ifndef __ContactPersonEmail_h__
#define __ContactPersonEmail_h__

#include "ContactPersonAddress.h"

namespace Msg
{
    class ContactPersonEmail;
    typedef std::shared_ptr<ContactPersonEmail> ContactPersonEmailRef;

    class ContactPersonEmail
        : public ContactPersonAddress
    {
        public:
            ContactPersonEmail(bool release, contacts_record_h record = nullptr);
            static const char *getUri();
            virtual int getId() const;
            virtual AddressType getAddressType() const;
            virtual int getPersonId() const;
            virtual std::string getDispName() const;
            virtual std::string getAddress() const;
            virtual std::string getThumbnailPath() const;
    };

    inline ContactPersonEmail::ContactPersonEmail(bool release, contacts_record_h record)
        : ContactPersonAddress(release, record)
    {
    }

    inline int ContactPersonEmail::getId() const
    {
        return getInt(_contacts_person_email.email_id);
    }

    inline int ContactPersonEmail::getPersonId() const
    {
        return getInt(_contacts_person_email.person_id);
    }

    inline std::string ContactPersonEmail::getDispName() const
    {
        return getStr(_contacts_person_email.display_name);
    }

    inline std::string ContactPersonEmail::getAddress() const
    {
        return getStr(_contacts_person_email.email);
    }

    inline std::string ContactPersonEmail::getThumbnailPath() const
    {
        return getStr(_contacts_person_email.image_thumbnail_path);
    }

    inline const char *ContactPersonEmail::getUri()
    {
        return _contacts_person_email._uri;
    }

    inline ContactPersonEmail::AddressType ContactPersonEmail::getAddressType() const
    {
        return EmailType;
    }
}


#endif /* __ContactPersonEmail_h__ */
