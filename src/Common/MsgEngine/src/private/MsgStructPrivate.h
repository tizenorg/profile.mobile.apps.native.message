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

#ifndef _MsgStructPrivate_H__
#define _MsgStructPrivate_H__

#include "MsgAddress.h"
#include "Logger.h"

#include <msg.h>

namespace Msg
{
    class MsgStructPrivate
    {
        public:
            inline MsgStructPrivate(bool release, msg_struct_t msgStruct = nullptr);
            inline virtual ~MsgStructPrivate();
            inline void set(msg_struct_t msgStruct);
            inline operator msg_struct_t() const;

        protected:
            msg_struct_t m_MsgStruct;
            bool m_Release;
    };

    inline MsgStructPrivate::MsgStructPrivate(bool release, msg_struct_t msgStruct)
        : m_MsgStruct(msgStruct)
        , m_Release(release)
    {
    }

    inline MsgStructPrivate::~MsgStructPrivate()
    {
        if(m_Release && m_MsgStruct)
        {
            msg_release_struct(&m_MsgStruct);
        }
    }

    inline void MsgStructPrivate::set(msg_struct_t msgStruct)
    {
        m_MsgStruct = msgStruct;
    }

    inline MsgStructPrivate::operator msg_struct_t() const
    {
        return m_MsgStruct;
    }
}

#endif /* _MsgStructPrivate_H__ */
