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

#ifndef ContactMyProfile_h_
#define ContactMyProfile_h_

#include "ContactRecord.h"

namespace Msg
{
    class ContactMyProfile;
    typedef std::shared_ptr<ContactMyProfile> ContactMyProfileRef;

    class ContactMyProfile
        : public ContactRecord
    {
        public:
            ContactMyProfile(bool release, contacts_record_h record = nullptr);
            int getId() const;
            std::string getThumbnailPath() const;
            std::string getDispName() const;
    };

    inline ContactMyProfile::ContactMyProfile(bool release, contacts_record_h record)
        : ContactRecord(release, record)
    {
    }

    inline int ContactMyProfile::getId() const
    {
        return getInt(_contacts_my_profile.id);
    }

    inline std::string ContactMyProfile::getThumbnailPath() const
    {
        return getStr(_contacts_my_profile.image_thumbnail_path);
    }

    inline std::string ContactMyProfile::getDispName() const
    {
        return getStr(_contacts_my_profile.display_name);
    }
}

#endif /* ContactMyProfile_h_ */
