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

#include <ContactRecord.h>

namespace Msg
{
    class ContactManager;

    class ContactPersonNumber
        : public ContactRecord
    {
        public:
            inline ContactPersonNumber(contacts_record_h record);
            inline static const char *getUri();
            inline int getId() const;
            inline int getPersonId() const;
            inline const char *getDispName() const;
            inline const char *getNumber() const;
            inline const char *getThumbnailPath() const;
    };

    inline ContactPersonNumber::ContactPersonNumber(contacts_record_h record)
        : ContactRecord(record)
    {
    }

    inline int ContactPersonNumber::getId() const
    {
        int id = 0;
        contacts_record_get_int(m_Record, _contacts_person_number.number_id, &id);
        return id;
    }

    inline int ContactPersonNumber::getPersonId() const
    {
        int id = 0;
        contacts_record_get_int(m_Record, _contacts_person_number.person_id, &id);
        return id;
    }

    inline const char *ContactPersonNumber::getDispName() const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, _contacts_person_number.display_name, &str);
        return str;
    }

    inline const char *ContactPersonNumber::getNumber() const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, _contacts_person_number.number, &str);
        return str;
    }

    inline const char *ContactPersonNumber::getThumbnailPath() const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, _contacts_person_number.image_thumbnail_path, &str);
        return str;
    }

    inline const char *ContactPersonNumber::getUri()
    {
        return _contacts_person_number._uri;
    }
}


#endif /* __ContactPersonNumber_h__ */
