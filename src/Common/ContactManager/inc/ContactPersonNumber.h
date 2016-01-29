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

#ifndef __ContactPersonNumber_h__
#define __ContactPersonNumber_h__

#include "ContactPersonAddress.h"

namespace Msg
{
    class ContactPersonNumber;
    typedef std::shared_ptr<ContactPersonNumber> ContactPersonNumberRef;

    class ContactPersonNumber
        : public ContactPersonAddress
    {
        public:
            ContactPersonNumber(bool release, contacts_record_h record = nullptr);
            static const char *getUri();
            virtual int getId() const;
            virtual AddressType getAddressType() const;
            virtual int getPersonId() const;
            virtual std::string getDispName() const;
            virtual std::string getAddress() const;
            virtual std::string getThumbnailPath() const;
    };

    inline ContactPersonNumber::ContactPersonNumber(bool release, contacts_record_h record)
        : ContactPersonAddress(release, record)
    {
    }

    inline int ContactPersonNumber::getId() const
    {
        return getInt(_contacts_person_number.number_id);
    }

    inline int ContactPersonNumber::getPersonId() const
    {
        return getInt(_contacts_person_number.person_id);
    }

    inline std::string ContactPersonNumber::getDispName() const
    {
        return getStr(_contacts_person_number.display_name);
    }

    inline std::string ContactPersonNumber::getAddress() const
    {
        return getStr(_contacts_person_number.number);
    }

    inline std::string ContactPersonNumber::getThumbnailPath() const
    {
        return getStr(_contacts_person_number.image_thumbnail_path);
    }

    inline const char *ContactPersonNumber::getUri()
    {
        return _contacts_person_number._uri;
    }

    inline ContactPersonNumber::AddressType ContactPersonNumber::getAddressType() const
    {
        return NumberType;
    }
}


#endif /* __ContactPersonNumber_h__ */
