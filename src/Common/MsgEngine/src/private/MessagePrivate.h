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

#ifndef _MESSAGE_PRIVATE_H__
#define _MESSAGE_PRIVATE_H__

#include "Message.h"

#include <msg_types.h>

namespace Msg
{
    class MessagePrivate:
        public virtual Message
    {
        public:
            MessagePrivate(msg_handle_t handle);
            virtual ~MessagePrivate();

            inline msg_struct_t getMsgStruct() const;
            virtual void setAddressList(const AddressList &addressList);
            virtual AddressList getAddressList() const;
            virtual ThreadId getThreadId() const;
            virtual time_t getTime() const;
            virtual std::string getNumber() const;

        protected:
            void setMsgStruct(msg_struct_t msgStruct);

        private:
            msg_struct_t m_pMsgStruct;
            msg_handle_t m_pHandle;
    };

    inline msg_struct_t MessagePrivate::getMsgStruct() const
    {
        return m_pMsgStruct;
    }
}

#endif /* _MESSAGE_PRIVATE_H__ */
