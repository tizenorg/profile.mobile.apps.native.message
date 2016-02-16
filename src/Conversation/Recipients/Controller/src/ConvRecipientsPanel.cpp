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

#include "ConvRecipientsPanel.h"
#include "MsgEngine.h"
#include "MsgUtils.h"
#include "ContactManager.h"
#include "CallbackAssist.h"

#include <notification.h>

using namespace Msg;

ConvRecipientsPanel::ConvRecipientsPanel(Evas_Object *parent, App &app)
    : ConvRecipientsPanelView(parent, app.getMsgEngine().getSettings().getAddressMaxLen())
    , m_App(app)
    , m_pListener(nullptr)
    , m_pMbe(nullptr)
{
    m_Picker.setListener(this);
    m_pMbe = new MbeRecipients(*this, m_App);
    m_pMbe->addSmartCb("item,added", SMART_CALLBACK(ConvRecipientsPanel, onMbeChanged), this);
    m_pMbe->addSmartCb("item,deleted", SMART_CALLBACK(ConvRecipientsPanel, onMbeChanged), this);
    m_pMbe->addSmartCb("item,clicked", SMART_CALLBACK(ConvRecipientsPanel, onMbeItemClicked), this);
    m_pMbe->show();
    setMbe(*m_pMbe);
}

ConvRecipientsPanel::~ConvRecipientsPanel()
{
    m_Picker.setListener(nullptr);
}

void ConvRecipientsPanel::read(Message &msg)
{
    auto list = m_pMbe->getItems();
    for(auto it : list)
    {
        MbeRecipientItem *recipItem = static_cast<MbeRecipientItem*>(it);
        MsgAddress &msgAddr = msg.addAddress();
        msgAddr.setAddress(recipItem->getAddress());
        msgAddr.setRecipientType(recipItem->getRecipType());
        msgAddr.setAddressType(recipItem->getAddressType());
    }
}

void ConvRecipientsPanel::write(const Message &msg)
{
    update(msg.getAddressList());
}

bool ConvRecipientsPanel::isMms() const
{
    auto items = m_pMbe->getItems();
    for(MbeRecipientItem *it : items)
    {
        MbeRecipientItem *item = static_cast<MbeRecipientItem*>(it);
        if(item->getAddressType() == MsgAddress::Email)
            return true;
    }
    return false;
}

void ConvRecipientsPanel::setListener(IConvRecipientsPanelListener *l)
{
    m_pListener = l;
}

void ConvRecipientsPanel::addRecipientsFromEntry()
{
    std::string text = getEntryText();
    TokenizedRecipients result = MsgUtils::tokenizeRecipients(text);
    bool duplicateFound = false;

    for(auto &it : result.validResults)
    {
        if(it.second == MsgAddress::Phone)
            it.first = MsgUtils::makeNormalizedNumber(it.first);

        duplicateFound |= appendItem(it.first, it.second) == MbeRecipients::DuplicatedStatus;
    }

    if(duplicateFound)
        showDuplicatedRecipientNotif();

    setEntryText(result.invalidResult);
}

void ConvRecipientsPanel::update(const MsgAddressList &addressList)
{
    clearMbe();
    int addrListLen = addressList.getLength();
    for(int i = 0; i < addrListLen; ++i)
    {
        auto &addr = addressList.at(i);
        appendItem(addr.getAddress(), addr.getAddressType());
    }
}

void ConvRecipientsPanel::update(const ThreadId &threadId)
{
    m_pMbe->update(threadId);
}

MbeRecipients::AppendItemStatus ConvRecipientsPanel::appendItem(const std::string &address,
        const std::string &dispName, MsgAddress::AddressType addressType)
{
    MbeRecipients::AppendItemStatus appendStatus = m_pMbe->appendItem(address, dispName, addressType);
    if(appendStatus == MbeRecipients::SuccessStatus && getEntryFocus())
        showMbe(true);
    return appendStatus;
}

MbeRecipients::AppendItemStatus ConvRecipientsPanel::appendItem(const std::string &address, MsgAddress::AddressType addressType)
{
    MbeRecipients::AppendItemStatus appendStatus = m_pMbe->appendItem(address, addressType);
    if(appendStatus == MbeRecipients::SuccessStatus && getEntryFocus())
        showMbe(true);
    return appendStatus;
}

void ConvRecipientsPanel::removeItem(MbeRecipientItem &item)
{
    m_pMbe->removeItem(item);
}

void ConvRecipientsPanel::editItem(MbeRecipientItem &item)
{
    showMbe(true);
    showEntry(true);
    setEntryText(item.getAddress());
    m_pMbe->removeItem(item);
}

void ConvRecipientsPanel::execCmd(const AppControlComposeRef &cmd)
{
    m_pMbe->clear();
    for(auto recipStr: cmd->getRecipientList())
    {
        appendItem(recipStr);
    }
    showMbe(!isMbeEmpty());
}

void ConvRecipientsPanel::onKeyDown(Evas_Event_Key_Down *ev)
{
    if(ev && ev->keyname)
    {
        MSG_LOG("Key: ", ev->keyname);

        if((strcmp(ev->keyname, "semicolon") == 0) || (strcmp(ev->keyname, "comma") == 0))
        {
            addRecipientsFromEntry();
        }
        else
        {
            if(m_pListener)
                m_pListener->onKeyDown(*this, *ev);
        }
    }
}

void ConvRecipientsPanel::onEntryFocusChanged()
{
    MSG_LOG("");
    if(getEntryFocus())
    {
        showMbe(!isMbeEmpty());
    }
    else
    {
        addRecipientsFromEntry();
        showMbe(false);
    }

    if(m_pListener)
        m_pListener->onEntryFocusChanged(*this);
}


void ConvRecipientsPanel::onContactButtonClicked()
{
    const int maxRecipientCount = m_App.getMsgEngine().getSettings().getMaxRecipientCount();
    int currentRecipientsCount = getItemsCount();
    if(currentRecipientsCount < maxRecipientCount)
        m_Picker.launch(maxRecipientCount - currentRecipientsCount);
    else
        showTooManyRecipientsNotif();
}


bool ConvRecipientsPanel::isRecipientExists(const std::string& address) const
{
    auto recipientList = m_pMbe->getItems();
    for(auto pViewItem : recipientList)
    {
        MbeRecipientItem *pItem = dynamic_cast<MbeRecipientItem*>(pViewItem);
        if(pItem && pItem->getAddress() == address)
            return true;
    }

    return false;
}

void ConvRecipientsPanel::onContactsPicked(const std::list<int> &numberIdList)
{
    bool duplicateFound = false;
    for(auto phoneNumId : numberIdList)
    {
        ContactPersonNumberRef num = m_App.getContactManager().getContactPersonNumber(phoneNumId);
        if(num)
            duplicateFound |= appendItem(num->getAddress(), num->getDispName(), MsgAddress::Phone) == MbeRecipients::DuplicatedStatus;
    }

    if(duplicateFound)
        showDuplicatedRecipientNotif();
    else
        setEntryFocus(true);
}

void ConvRecipientsPanel::onPopupBtnClicked(Popup &popup, int buttonId)
{
    setEntryFocus(true);
    popup.destroy();
}

void ConvRecipientsPanel::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    setEntryFocus(true);
}

void ConvRecipientsPanel::showDuplicatedRecipientNotif()
{
    notification_status_message_post(msg("IDS_MSGC_BODY_DUPLICATED_RECIPIENT").cStr());
}

void ConvRecipientsPanel::showTooManyRecipientsNotif()
{
    int maxRecipientCount = m_App.getMsgEngine().getSettings().getMaxRecipientCount();
    notification_status_message_post(msgArgs("IDS_MSGC_BODY_MAXIMUM_NUMBER_OF_RECIPIENTS_HPD_REACHED", maxRecipientCount).cStr());
}

void ConvRecipientsPanel::onMbeChanged(Evas_Object *oj, void *eventInfo)
{
    if(m_pListener)
        m_pListener->onMbeChanged(*this);
}

void ConvRecipientsPanel::onMbeItemClicked(Evas_Object *oj, void *eventInfo)
{
    if(m_pListener)
        m_pListener->onItemClicked(*this, *ViewItem::staticCast<MbeRecipientItem*>(eventInfo));
}
