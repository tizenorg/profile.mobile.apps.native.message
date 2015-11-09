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

#ifndef MsgAddressPrivate_h_
#define MsgAddressPrivate_h_

#include "MsgAddress.h"
#include "MsgListHandlePrivate.h"
#include "MsgStructListPrivate.h"
#include "MsgStructPrivate.h"

namespace Msg
{
    class MsgAddressPrivate
        : public MsgStructPrivate
        , public MsgAddress
    {
        public:
            MsgAddressPrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgAddressPrivate();

            virtual std::string getAddress() const;
            virtual AddressType getAddressType() const;
            virtual RecipientType getRecipientType() const;

            virtual void setAddress(const std::string &address);
            virtual void setAddressType(AddressType type);
            virtual void setRecipientType(RecipientType type);

            void setFields(const MsgAddress &addr);
    };

    typedef class MsgListHandlePrivate<MsgAddressPrivate, MsgAddress> MsgAddressListHandlePrivate;
    typedef class MsgStructListPrivate<MsgAddressPrivate, MsgAddress> MsgAddressStructListPrivate;
}

#endif /* MsgAddressPrivate_h_ */
