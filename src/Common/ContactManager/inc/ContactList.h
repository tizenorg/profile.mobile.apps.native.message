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

#ifndef __ContactList_H__
#define __ContactList_H__

#include <contacts.h>

namespace Msg
{
    class ContactManager;

    template <typename T>
    class ContactList
    {
            friend class ContactManager;

        public:
            ContactList(contacts_list_h list)
                : m_List(list)
                , m_Record(false)
            {
            }

            ~ContactList()
            {
                if(m_List)
                    contacts_list_destroy(m_List, true);
            }

            bool next()
            {
                return contacts_list_next(m_List) == 0;
            }

            bool prev()
            {
                return contacts_list_prev(m_List) == 0;
            }

            T &get()
            {
                contacts_record_h rec = nullptr;
                contacts_list_get_current_record_p(m_List, &rec);
                m_Record.set(rec);
                return m_Record;
            }

            int getCount() const
            {
                int count = 0;
                contacts_list_get_count(m_List, &count);
                return count;
            }

            bool isEmpty() const
            {
                return getCount() <= 0;
            }

        private:
            contacts_list_h m_List;
            T m_Record;
    };
}

#endif /* __ContactList_H__ */
