/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef PopupRecipientListItem_h_
#define PopupRecipientListItem_h_

#include "PopupListItem.h"
#include "ContactAddress.h"

namespace Msg
{
    class PopupAddressListItem
        : public PopupTextListItem
    {
    public:
        PopupAddressListItem(PopupList &parent,
                            const std::string &text,
                            const std::string &address,
                            PopupListItemPressedCb cb,
                            void *userData);

        virtual ~PopupAddressListItem();
        std::string getAddress() const;

    private:
        std::string m_Address;
    };

    class PopupPersonIdListItem
        : public PopupTextListItem
    {
    public:
        PopupPersonIdListItem(PopupList &parent,
                              const std::string &text,
                              int contactId,
                              ContactAddress::OwnerType ownerType,
                              PopupListItemPressedCb cb,
                              void *userData);

        virtual ~PopupPersonIdListItem();

        int getContactId() const;
        ContactAddress::OwnerType getContactOwnerType() const;

    private:
        int m_ContactId;
        ContactAddress::OwnerType m_OwnerType;
    };
}

#endif /* PopupRecipientListItem_h_ */
