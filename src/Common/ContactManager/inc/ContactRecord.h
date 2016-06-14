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

#ifndef __ContactRecord_h__
#define __ContactRecord_h__

#include <contacts.h>
#include <string>
#include <memory>

namespace Msg
{
    class ContactRecord;
    typedef std::shared_ptr<ContactRecord> ContactRecordRef;

    class ContactRecord
    {
        public:
            ContactRecord(bool release, contacts_record_h record = nullptr);
            virtual ~ContactRecord();
            ContactRecord(ContactRecord&) = delete;
            ContactRecord& operator=(ContactRecord&) = delete;

            void set(contacts_record_h record);
            contacts_record_h getRecord() const;

        protected:
            std::string getStr(unsigned propertyId) const;
            int getInt(unsigned propertyId) const;

        private:
            contacts_record_h m_Record;
            bool m_Release;
    };

    inline void ContactRecord::set(contacts_record_h record)
    {
        m_Record = record;
    }

    inline std::string ContactRecord::getStr(unsigned propertyId) const
    {
        char *str = nullptr;
        contacts_record_get_str_p(m_Record, propertyId, &str);
        return str ? str : std::string();
    }

    inline int ContactRecord::getInt(unsigned propertyId) const
    {
        int val = 0;
        contacts_record_get_int(m_Record, propertyId, &val);
        return val;
    }

    inline contacts_record_h ContactRecord::getRecord() const
    {
        return m_Record;
    }

}

#endif /* __ContactRecord_h__ */
