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

#include "MbeRecipients.h"
#include "ContactManager.h"
#include "MsgUtils.h"
#include "Logger.h"

using namespace Msg;

MbeRecipients::MbeRecipients(Evas_Object *parent, App &app)
    : MbeRecipientsView(parent)
    , m_App(app)
{
    m_App.getContactManager().addListener(*this);
}

MbeRecipients::~MbeRecipients()
{
    m_App.getContactManager().removeListener(*this);
}

std::string MbeRecipients::getDispName(const std::string &address) const
{
    std::string dispName;
    ContactPersonAddressRef contactAddress = m_App.getContactManager().getContactPersonAddress(address);
    if(contactAddress)
        dispName = contactAddress->getDispName();
    if(dispName.empty())
        dispName = address;
    return dispName;
}

void MbeRecipients::updateItemsDispName()
{
    auto items = getItems();
    for(MbeRecipientItem *item: items)
    {
        item->setDispName(getDispName(item->getAddress()));
    }
}

void MbeRecipients::update(const MsgAddressList &addressList)
{
    clear();
    int addrListLen = addressList.getLength();
    for(int i = 0; i < addrListLen; i++)
    {
        const MsgAddress &addr = addressList.at(i);
        appendItem(addr.getAddress(), addr.getAddressType());
    }
}

void MbeRecipients::update(ThreadId threadId)
{
    clear();
    if(threadId.isValid())
    {
        MsgAddressListRef addrList = m_App.getMsgEngine().getStorage().getAddressList(threadId);
        if(addrList)
            update(*addrList);
    }
}

MbeRecipients::AppendItemStatus MbeRecipients::appendItem(const std::string &address, MsgAddress::AddressType addressType)
{
    return appendItem(address, getDispName(address), addressType);
}

MbeRecipients::AppendItemStatus MbeRecipients::appendItem(const std::string &address, const std::string &dispName, MsgAddress::AddressType addressType)
{
    if(getItemsCount() >= m_App.getMsgEngine().getSettings().getMaxRecipientCount())
        return TooManyRecipStatus;

    AppendItemStatus result = SuccessStatus;
    if(!isRecipientExists(address))
    {
        if(addressType == MsgAddress::UnknownAddressType)
            addressType = MsgUtils::getAddressType(address);

        if(addressType == MsgAddress::Phone || addressType == MsgAddress::Email)
        {
            result = SuccessStatus;
            MbeRecipientItem *item = new MbeRecipientItem(address, dispName, addressType);
            MbeRecipientsView::appendItem(*item);
        }
        else
        {
            result = InvalidRecipStatus;
            MSG_LOG("invalid recipient: ", address);
        }
    }
    else
    {
        result = DuplicatedStatus;
    }
    return result;
}

bool MbeRecipients::isRecipientExists(const std::string& address) const
{
    auto recipientList = getItems();
    for(auto pViewItem : recipientList)
    {
        MbeRecipientItem *pItem = dynamic_cast<MbeRecipientItem*>(pViewItem);
        if(pItem && pItem->getAddress() == address)
            return true;
    }

    return false;
}

void MbeRecipients::onContactChanged()
{
    MSG_LOG("");
    updateItemsDispName();
}


