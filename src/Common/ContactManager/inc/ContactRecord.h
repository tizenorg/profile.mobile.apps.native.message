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
            int getChildCount(unsigned propertyId) const;
            contacts_record_h getChildP(unsigned propertyId, int index) const;

            static std::string getStr(contacts_record_h rec, unsigned propertyId);
            static int getInt(contacts_record_h rec, unsigned propertyId);
            static int getChildCount(contacts_record_h rec, unsigned propertyId);
            static contacts_record_h getChildP(contacts_record_h rec, unsigned propertyId, int index);

        private:
            contacts_record_h m_Record;
            bool m_Release;
    };

    inline void ContactRecord::set(contacts_record_h record)
    {
        m_Record = record;
    }

    inline contacts_record_h ContactRecord::getRecord() const
    {
        return m_Record;
    }

    inline std::string ContactRecord::getStr(contacts_record_h rec, unsigned propertyId)
    {
        char *str = nullptr;
        contacts_record_get_str_p(rec, propertyId, &str);
        return str ? str : std::string();
    }

    inline int ContactRecord::getChildCount(contacts_record_h rec, unsigned propertyId)
    {
        int count = 0;
        contacts_record_get_child_record_count(rec, propertyId, &count);
        return count;
    }

    inline contacts_record_h ContactRecord::getChildP(contacts_record_h rec, unsigned propertyId, int index)
    {
        contacts_record_h res = {};
        contacts_record_get_child_record_at_p(rec, propertyId, index, &res);
        return res;
    }

    inline int ContactRecord::getInt(contacts_record_h rec, unsigned propertyId)
    {
        int val = 0;
        contacts_record_get_int(rec, propertyId, &val);
        return val;
    }

    inline std::string ContactRecord::getStr(unsigned propertyId) const
    {
        return getStr(m_Record, propertyId);
    }

    inline int ContactRecord::getChildCount(unsigned propertyId) const
    {
        return getChildCount(m_Record, propertyId);
    }

    inline contacts_record_h ContactRecord::getChildP(unsigned propertyId, int index) const
    {
        return getChildP(m_Record, propertyId, index);
    }

    inline int ContactRecord::getInt(unsigned propertyId) const
    {
        return getInt(m_Record, propertyId);
    }
}

#endif /* __ContactRecord_h__ */
