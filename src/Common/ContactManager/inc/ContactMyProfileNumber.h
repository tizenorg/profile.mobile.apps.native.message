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

#ifndef __ContactMyProfileNumber_h__
#define __ContactMyProfileNumber_h__

#include "ContactAddress.h"
#include "ContactRecord.h"
#include "ContactMyProfile.h"

namespace Msg
{
    class ContactMyProfileNumber;
    typedef std::shared_ptr<ContactMyProfileNumber> ContactMyProfileNumberRef;

    class ContactMyProfileNumber
        : public ContactAddress
        , public ContactRecord
    {
        public:
            ContactMyProfileNumber(bool release, const ContactMyProfile &myProfile, contacts_record_h record = nullptr);
            static const char *getUri();
            int getId() const;

            // ContactAddress:
            virtual AddressType getAddressType() const;
            virtual OwnerType getOwnerType() const;
            virtual int getOwnerId() const;
            virtual std::string getDispName() const;
            virtual std::string getAddress() const;
            virtual std::string getThumbnailPath() const;

        private:
            int m_MyProfileId;
            std::string m_ThumbnailPath;
            std::string m_DispName;
    };

    inline ContactMyProfileNumber::ContactMyProfileNumber(bool release, const ContactMyProfile &myProfile, contacts_record_h record)
        : ContactRecord(release, record)
        , m_MyProfileId(myProfile.getId())
        , m_ThumbnailPath(myProfile.getThumbnailPath())
        , m_DispName(myProfile.getDispName())
    {
    }

    inline int ContactMyProfileNumber::getId() const
    {
        return getInt(_contacts_number.id);
    }

    inline int ContactMyProfileNumber::getOwnerId() const
    {
        return m_MyProfileId;
    }

    inline std::string ContactMyProfileNumber::getDispName() const
    {
        return m_DispName;
    }

    inline std::string ContactMyProfileNumber::getAddress() const
    {
        return getStr(_contacts_number.number);
    }

    inline std::string ContactMyProfileNumber::getThumbnailPath() const
    {
        return m_ThumbnailPath;
    }

    inline const char *ContactMyProfileNumber::getUri()
    {
        return _contacts_number._uri;
    }

    inline ContactMyProfileNumber::OwnerType ContactMyProfileNumber::getOwnerType() const
    {
        return MyProfileType;
    }

    inline ContactMyProfileNumber::AddressType ContactMyProfileNumber::getAddressType() const
    {
        return NumberType;
    }
}


#endif /* __ContactMyProfileNumber_h__ */
