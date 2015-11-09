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

#ifndef _MsgStructListPrivate_H__
#define _MsgStructListPrivate_H__

#include "MsgList.h"

#include <msg.h>

namespace Msg
{
    static msg_struct_list_s emptyStructList;

    // class T : public BaseT {}
    template<typename T, typename BaseT>
    class MsgStructListPrivate
        : public MsgList<BaseT> // implement interface
    {
        public:
            MsgStructListPrivate(bool release, const msg_struct_list_s &list = emptyStructList)
                : m_List(list)
                , m_Object(false) // release = false
                , m_Release(release)
            {
            }

            virtual ~MsgStructListPrivate()
            {
                if(m_Release && m_List.nCount)
                {
                    msg_release_list_struct(&m_List);
                }
            }

            virtual BaseT &at(int i)
            {
                msg_struct_t msgStruct = m_List.msg_struct_info[i];
                m_Object.set(msgStruct);
                return m_Object;
            }

            virtual const BaseT &at(int i) const
            {
                msg_struct_t msgStruct = m_List.msg_struct_info[i];
                m_Object.set(msgStruct);
                return m_Object;
            }

            virtual int getLength() const
            {
                return m_List.nCount;
            }

            inline void set(msg_struct_list_s &list)
            {
                m_List = list;
            }

            inline operator msg_struct_list_s &()
            {
                return m_List;
            }

            inline operator const msg_struct_list_s &() const
            {
                return m_List;
            }

            inline msg_struct_list_s &get()
            {
                return m_List;
            }

            inline const msg_struct_list_s &get() const
            {
                return m_List;
            }

            MsgStructListPrivate(MsgStructListPrivate&) = delete;
            MsgStructListPrivate &operator=(MsgStructListPrivate&) = delete;

        protected:
            mutable msg_struct_list_s m_List;
            mutable T m_Object;
            bool m_Release;
    };
}

#endif /* _MsgStructListPrivate_H__ */
