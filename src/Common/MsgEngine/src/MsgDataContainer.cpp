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

#include "MsgDataContainer.h"
#include <algorithm>

using namespace Msg;

MsgDataContainer::MsgDataContainer()
    : m_Type(Message::MT_Unknown)
{

}

MsgDataContainer::~MsgDataContainer()
{

}

void  MsgDataContainer::setType(Message::Type type)
{
    m_Type = type;
}

Message::Type MsgDataContainer::getType() const
{
    return m_Type;
}

void MsgDataContainer::setText(const std::string &text)
{
    m_Text = text;
}

const std::string &MsgDataContainer::getText() const
{
    return m_Text;
}

const AddressList& MsgDataContainer::getAddressList() const
{
    return m_AddressList;
}

void MsgDataContainer::addAddressList(const AddressList &newList)
{
    m_AddressList = newList;
}

void MsgDataContainer::addAddress(const std::string& address)
{
    auto itr = std::find(m_AddressList.begin(), m_AddressList.end(), address);
    if(itr == m_AddressList.end())
    {
        m_AddressList.push_back(address);
    }
}

void MsgDataContainer::removeAddress(const std::string& address)
{
    auto itr = std::find(m_AddressList.begin(), m_AddressList.end(), address);
    if(itr != m_AddressList.end())
    {
        m_AddressList.erase(itr);
    }
}
