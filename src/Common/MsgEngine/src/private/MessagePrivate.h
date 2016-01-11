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
#include "MsgStructPrivate.h"
#include "MsgAddressPrivate.h"

#include <msg_types.h>

namespace Msg
{
    class MessagePrivate
        : public virtual Message
        , public MsgStructPrivate
    {
        public:
            MessagePrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MessagePrivate();

            virtual MsgId getId() const;
            virtual ThreadId getThreadId() const;
            virtual time_t getTime() const;
            virtual const MsgAddressListHandlePrivate &getAddressList() const;
            virtual MsgAddressPrivate &addAddress();
            virtual void addAddresses(const MsgAddressList &list);
            virtual void setText(const std::string &text);
            virtual std::string getText() const;
            virtual Type getType() const;

            // FIXME: internal compiler error(arm only)
            // virtual std::string getSubject() const;
            // virtual void setSubject(const std::string &text);

            void setId(MsgId id);
            void set(msg_struct_t msgStruct);
            virtual void commit();

        private:
            bool isMms() const;

        protected:
            mutable MsgAddressPrivate m_Address;
            mutable MsgAddressListHandlePrivate m_AddressList;
    };

    typedef class MsgListHandlePrivate<MessagePrivate, Message> MessageListHandlePrivate;
    typedef class MsgStructListPrivate<MessagePrivate, Message> MessageStructListPrivate;
}

#endif /* _MESSAGE_PRIVATE_H__ */
