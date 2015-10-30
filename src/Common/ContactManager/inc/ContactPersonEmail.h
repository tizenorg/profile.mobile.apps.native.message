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

#include <ContactRecord.h>

namespace Msg
{
    class ContactPersonEmail
        : public ContactRecord
    {
        public:
            inline ContactPersonEmail(contacts_record_h record);
            inline static const char *getUri();
            inline int getId() const;
            inline int getPersonId() const;
            inline const char *getDispName() const;
            inline const char *getEmail() const;
            inline const char *getThumbnailPath() const;
    };

    inline ContactPersonEmail::ContactPersonEmail(contacts_record_h record)
        : ContactRecord(record)
    {
    }

    inline int ContactPersonEmail::getId() const
    {
        int id = 0;
        contacts_record_get_int(m_Record, _contacts_person_email.email_id, &id);
        return id;
    }

    inline int ContactPersonEmail::getPersonId() const
    {
        int id = 0;
        contacts_record_get_int(m_Record, _contacts_person_email.person_id, &id);
        return id;
    }

    inline const char *ContactPersonEmail::getDispName() const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, _contacts_person_email.display_name, &str);
        return str;
    }

    inline const char *ContactPersonEmail::getEmail() const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, _contacts_person_email.email, &str);
        return str;
    }

    inline const char *ContactPersonEmail::getThumbnailPath() const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, _contacts_person_email.image_thumbnail_path, &str);
        return str;
    }

    inline const char *ContactPersonEmail::getUri()
    {
        return _contacts_person_email._uri;
    }
}


#endif /* __ContactPersonEmail_h__ */
