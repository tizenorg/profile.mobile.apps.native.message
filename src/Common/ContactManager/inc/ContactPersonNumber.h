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

#include "ContactAddress.h"
#include "ContactRecord.h"

namespace Msg
{
    class ContactPersonNumber;
    typedef std::shared_ptr<ContactPersonNumber> ContactPersonNumberRef;

    class ContactPersonNumber
        : public ContactAddress
        , public ContactRecord
    {
        public:
            ContactPersonNumber(bool release, contacts_record_h record = nullptr);
            static const char *getUri();
            int getNumberId() const;

            // ContactAddress:
            virtual OwnerType getOwnerType() const;
            virtual AddressType getAddressType() const;
            virtual int getOwnerId() const;
            virtual std::string getDispName() const;
            virtual std::string getAddress() const;
            virtual std::string getThumbnailPath() const;
    };

    inline ContactPersonNumber::ContactPersonNumber(bool release, contacts_record_h record)
        : ContactRecord(release, record)
    {
    }

    inline int ContactPersonNumber::getNumberId() const
    {
        return getInt(_contacts_person_number.number_id);
    }

    inline int ContactPersonNumber::getOwnerId() const
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

    inline ContactPersonNumber::OwnerType ContactPersonNumber::getOwnerType() const
    {
        return PersonType;
    }

    inline ContactPersonNumber::AddressType ContactPersonNumber::getAddressType() const
    {
        return NumberType;
    }
}


#endif /* __ContactPersonNumber_h__ */
