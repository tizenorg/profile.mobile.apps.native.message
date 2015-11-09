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

#ifndef MsgAddress_h_
#define MsgAddress_h_

#include "MsgList.h"

#include <memory>
#include <string>

namespace Msg
{
    class MsgAddress;
    typedef MsgList<MsgAddress> MsgAddressList;
    typedef std::shared_ptr<MsgList<MsgAddress>> MsgAddressListRef;

    class MsgAddress
    {
        public:
            enum AddressType
            {
                Phone,
                Email,
                UnknownAddressType
            };

            enum RecipientType
            {
                To,
                Cc,
                Bcc,
                UnknownRecipientType
            };

        public:
            virtual ~MsgAddress() {};

            virtual std::string getAddress() const = 0;
            virtual AddressType getAddressType() const = 0;
            virtual RecipientType getRecipientType() const = 0;

            virtual void setAddress(const std::string &address) = 0;
            virtual void setAddressType(AddressType type) = 0;
            virtual void setRecipientType(RecipientType type) = 0;
    };

}

#endif /* MsgAddress_h_ */
