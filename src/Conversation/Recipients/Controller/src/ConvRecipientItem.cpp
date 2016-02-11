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

#include "ConvRecipientItem.h"

using namespace Msg;


ConvRecipientItem::ConvRecipientItem(const std::string &address,
                             MsgAddress::AddressType addressType,
                             MsgAddress::RecipientType recipientType)
    : m_Address(address)
    , m_AddressType(addressType)
    , m_RecipientType(recipientType)
{
}

ConvRecipientItem::~ConvRecipientItem()
{
}

const std::string &ConvRecipientItem::getAddress() const
{
    return m_Address;
}

MsgAddress::AddressType ConvRecipientItem::getAddressType() const
{
    return m_AddressType;
}

MsgAddress::RecipientType ConvRecipientItem::getRecipientType() const
{
    return m_RecipientType;
}

void ConvRecipientItem::setAddress(const std::string &address)
{
    m_Address = address;
}

void ConvRecipientItem::setRecipientType(MsgAddress::RecipientType type)
{
    m_RecipientType = type;
}

void ConvRecipientItem::setAddressType(MsgAddress::AddressType type)
{
    m_AddressType = type;
}


