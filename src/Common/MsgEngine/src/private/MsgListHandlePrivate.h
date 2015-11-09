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

#ifndef _MsgListHandlePrivate_H__
#define _MsgListHandlePrivate_H__

#include "MsgList.h"

#include <msg.h>

namespace Msg
{
    // class T : public BaseT {}
    template<typename T, typename BaseT>
    class MsgListHandlePrivate
        : public MsgList<BaseT> // implement interface
    {
        public:
            MsgListHandlePrivate(bool release, msg_list_handle_t list = nullptr)
                : m_List(list)
                , m_Object(false)
                , m_Release(release)
            {
            }

            virtual ~MsgListHandlePrivate()
            {
                if(m_Release && m_List)
                {
                    msg_list_free(m_List);
                }
            }

            virtual BaseT &at(int i)
            {
                msg_struct_t msgStruct = msg_list_nth_data(m_List, i);
                m_Object.set(msgStruct);
                return m_Object;
            }

            virtual const BaseT &at(int i) const
            {
                msg_struct_t msgStruct = msg_list_nth_data(m_List, i);
                m_Object.set(msgStruct);
                return m_Object;
            }

            virtual int getLength() const
            {
                return msg_list_length(m_List);
            }

            inline void set(msg_list_handle_t list)
            {
                m_List = list;
            }

            inline operator msg_list_handle_t &()
            {
                return m_List;
            }

            inline operator const msg_list_handle_t &() const
            {
                return m_List;
            }

            inline const msg_list_handle_t &get() const
            {
                return m_List;
            }

            inline msg_list_handle_t &get()
            {
                return m_List;
            }

        protected:
            mutable msg_list_handle_t m_List;
            mutable T m_Object;
            bool m_Release;
    };
}

#endif /* _MsgListHandlePrivate_H__ */
