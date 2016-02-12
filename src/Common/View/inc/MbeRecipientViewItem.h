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
 *
 */

#ifndef MbeRecipientItem_h_
#define MbeRecipientItem_h_

#include "ViewItem.h"
#include "MsgAddress.h"

namespace Msg
{
    class MbeRecipientsView;

    class MbeRecipientItem
        : public ViewItem
    {
        friend class MbeRecipientsView;

        public:
            MbeRecipientItem(const std::string &address, const std::string &dispName,
                    MsgAddress::AddressType addressType, MsgAddress::RecipientType recipType = MsgAddress::To);

            const std::string &getDispName() const;
            const std::string &getAddress() const;
            MsgAddress::AddressType getAddressType() const;
            MsgAddress::RecipientType getRecipType() const;

        private:
            std::string m_Address;
            std::string m_DispName;
            MsgAddress::AddressType m_AddressType;
            MsgAddress::RecipientType m_RecipType;
    };

    inline MbeRecipientItem::MbeRecipientItem(const std::string &address, const std::string &dispName,
            MsgAddress::AddressType addressType, MsgAddress::RecipientType recipType)
        : m_Address(address)
        , m_DispName(dispName)
        , m_AddressType(addressType)
        , m_RecipType(recipType)
    {
    }

    inline const std::string &MbeRecipientItem::getDispName() const
    {
        return m_DispName;
    }

    inline const std::string &MbeRecipientItem::getAddress() const
    {
        return m_Address;
    }

    inline MsgAddress::AddressType MbeRecipientItem::getAddressType() const
    {
        return m_AddressType;
    }

    inline MsgAddress::RecipientType MbeRecipientItem::getRecipType() const
    {
        return m_RecipType;
    }
}

#endif /* MbeRecipientItem_h_ */
