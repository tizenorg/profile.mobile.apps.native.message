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

#ifndef __ContactPersonPhoneLog_h__
#define __ContactPersonPhoneLog_h__

#include <ContactRecord.h>

namespace Msg
{
    class ContactManager;

    class ContactPersonPhoneLog
        : public ContactRecord
    {
        public:
            ContactPersonPhoneLog(contacts_record_h record);
            static const char *getUri();
            int getId() const;
            int getPersonId() const;
            std::string getDispName() const;
            std::string getAddress() const;
    };

    inline ContactPersonPhoneLog::ContactPersonPhoneLog(contacts_record_h record)
        : ContactRecord(record)
    {
    }

    inline int ContactPersonPhoneLog::getId() const
    {
        return getInt(_contacts_person_phone_log.log_id);
    }

    inline int ContactPersonPhoneLog::getPersonId() const
    {
        return getInt(_contacts_person_phone_log.person_id);
    }

    inline std::string ContactPersonPhoneLog::getDispName() const
    {
        return getStr(_contacts_person_phone_log.display_name);
    }

    inline std::string ContactPersonPhoneLog::getAddress() const
    {
        return getStr(_contacts_person_phone_log.address);
    }

    inline const char *ContactPersonPhoneLog::getUri()
    {
        return _contacts_person_phone_log._uri;
    }
}


#endif /* __ContactPersonPhoneLog_h__ */
