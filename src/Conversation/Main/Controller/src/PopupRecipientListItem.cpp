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

#include "PopupRecipientListItem.h"

using namespace Msg;

PopupAddressListItem::PopupAddressListItem(PopupList &parent,
                                           const std::string &text,
                                           const std::string &address,
                                           PopupListItemPressedCb cb,
                                           void *userData)
    : PopupTextListItem(parent, text, cb, userData)
    , m_Address(address)
{
}

PopupAddressListItem::~PopupAddressListItem()
{
}

std::string PopupAddressListItem::getAddress() const
{
    return m_Address;
}


PopupPersonIdListItem::PopupPersonIdListItem(PopupList &parent,
                                             const std::string &text,
                                             int contactId,
                                             ContactAddress::OwnerType ownerType,
                                             PopupListItemPressedCb cb,
                                             void *userData)
    : PopupTextListItem(parent, text, cb, userData)
    , m_ContactId(contactId)
    , m_OwnerType(ownerType)
{
}

PopupPersonIdListItem::~PopupPersonIdListItem()
{
}

int PopupPersonIdListItem::getContactId() const
{
    return m_ContactId;
}

ContactAddress::OwnerType PopupPersonIdListItem::getContactOwnerType() const
{
    return m_OwnerType;
}
