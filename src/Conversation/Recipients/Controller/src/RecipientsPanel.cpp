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

#include "RecipientsPanel.h"
#include "MsgEngine.h"
#include "MsgUtils.h"

using namespace Msg;

RecipientsPanel::RecipientsPanel(Evas_Object *parent, App &app)
    : RecipientsPanelView(parent, app.getMsgEngine().getSettings().getAddressMaxLen())
    , m_App(app)
    , m_pListener(nullptr)
{
}

RecipientsPanel::~RecipientsPanel()
{

}

void RecipientsPanel::read(Message &msg)
{
    RecipientViewItemList list = getItems();
    for(auto it : list)
    {
        RecipientItem *recipItem = static_cast<RecipientItem*>(it);
        MsgAddress &msgAddr = msg.addAddress();
        msgAddr.setAddress(recipItem->getAddress());
        msgAddr.setRecipientType(recipItem->getRecipientType());
        msgAddr.setAddressType(recipItem->getAddressType());
    }
}

bool RecipientsPanel::isMms() const
{
    auto items = getItems();
    for(RecipientViewItem *it : items)
    {
        RecipientItem *item = static_cast<RecipientItem*>(it);
        if(item->getAddressType() == MsgAddress::Email)
            return true;
    }
    return false;
}

void RecipientsPanel::setListener(IRecipientsPanelListener *l)
{
    m_pListener = l;
}

void RecipientsPanel::addRecipients()
{
    std::string text = getEntryText();
    TokenizedRecipients result = MsgUtils::tokenizeRecipients(text);

    for(auto & it : result.validResults)
    {
        if(it.second == MsgAddress::Phone)
            it.first = MsgUtils::makeNormalizedNumber(it.first);

        appendItem(it.first, it.first, it.second);
    }
    setEntryText(result.invalidResult);
}

bool RecipientsPanel::appendItem(const std::string &address, const std::string &dispName, MsgAddress::AddressType addressType)
{
    bool result = false;
    if(addressType == MsgAddress::UnknownAddressType)
    {
        addressType = MsgUtils::getAddressType(address);
    }
    if(addressType == MsgAddress::Phone || addressType == MsgAddress::Email)
    {
        result = true;
        RecipientItem *item = new RecipientItem(address, addressType);
        item->setDisplayName(dispName);
        RecipientsPanelView::appendItem(*item);

        if(getEntryFocus())
            showMbe(true);
    }
    else
    {
        MSG_LOG("invalid recipient: ", address);
    }
    return result;
}

void RecipientsPanel::onKeyDown(Evas_Event_Key_Down *ev)
{
    if(ev && ev->keyname)
    {
        MSG_LOG("Key: ", ev->keyname);

        if((strcmp(ev->keyname, "semicolon") == 0) || (strcmp(ev->keyname, "comma") == 0))
        {
            addRecipients();
        }
        else
        {
            if(m_pListener)
                m_pListener->onKeyDown(*this, *ev);
        }
    }
}

void RecipientsPanel::onEntryFocusChanged()
{
    MSG_LOG("");
    if(getEntryFocus())
    {
        showMbe(!isMbeEmpty());
    }
    else
    {
        addRecipients();
        showMbe(false);
    }

    if(m_pListener)
        m_pListener->onEntryFocusChanged(*this);
}

void RecipientsPanel::onItemAdded(RecipientViewItem &item)
{
    if(m_pListener)
        m_pListener->onItemAdded(*this, static_cast<RecipientItem&>(item));
}

void RecipientsPanel::onItemDeleted(RecipientViewItem &item)
{
    if(m_pListener)
        m_pListener->onItemDeleted(*this, static_cast<RecipientItem&>(item));
}

void RecipientsPanel::onContactButtonClicked()
{
    MSG_LOG("");
}

void RecipientsPanel::onItemSelected(RecipientViewItem &item)
{
    MSG_LOG("");
}

void RecipientsPanel::onItemPressed(RecipientViewItem &item)
{
    MSG_LOG("");
}

void RecipientsPanel::onItemClicked(RecipientViewItem &item)
{
    MSG_LOG("");
}

