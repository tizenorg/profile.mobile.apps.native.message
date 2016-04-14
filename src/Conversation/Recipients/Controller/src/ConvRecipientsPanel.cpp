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
{
    m_Picker.setListener(this);
    m_pMbe = new MbeRecipients(*this, m_App);
    m_pMbe->setListener(this);
    m_pMbe->show();
    setMbe(m_pMbe);
    m_App.getContactManager().addListener(*this);
}

ConvRecipientsPanel::~ConvRecipientsPanel()
{
    m_Picker.setListener(nullptr);
    m_App.getContactManager().removeListener(*this);
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

void ConvRecipientsPanel::addRecipientsFromEntry(bool showPopup)
{
    MSG_LOG("");
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
    if(!result.invalidResult.empty() && showPopup)
        showInvalidRecipientsPopup();
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
    auto appendStatus = m_pMbe->appendItem(address, dispName, addressType);
    appendStatusHandler(appendStatus);
    return appendStatus;
}

MbeRecipients::AppendItemStatus ConvRecipientsPanel::appendItem(const std::string &address, MsgAddress::AddressType addressType)
{
    auto appendStatus = m_pMbe->appendItem(address, addressType);
    appendStatusHandler(appendStatus);
    return appendStatus;
}

void ConvRecipientsPanel::removeSelectedItem()
{
    MbeRecipientItem* pItem = getSelectedItem();
    if(pItem)
        pItem->destroy();
}

void ConvRecipientsPanel::editSelectedItem()
{
    MbeRecipientItem* pItem = getSelectedItem();
    if(pItem)
    {
        showMbe(true);
        showEntry(true);
        setEntryText(pItem->getAddress());
        removeSelectedItem();
    }
}

void ConvRecipientsPanel::execCmd(const AppControlComposeRef &cmd)
{
    m_pMbe->clear();
    for(auto recipStr: cmd->getRecipientList())
    {
        appendItem(recipStr);
    }
}

int ConvRecipientsPanel::getMaxRecipientCount() const
{
    return m_App.getMsgEngine().getSettings().getMaxRecipientCount();
}

void ConvRecipientsPanel::appendStatusHandler(MbeRecipients::AppendItemStatus status)
{
    switch(status)
    {
        case MbeRecipients::SuccessStatus:
        if(getEntryFocus())
            showMbe(true);
            break;
        case MbeRecipients::DuplicatedStatus:
            showDuplicatedRecipientNotif();
            break;
        case MbeRecipients::TooManyRecipStatus:
            showTooManyRecipientsNotif();
            break;
        case MbeRecipients::InvalidRecipStatus:
        default:
            showInvalidRecipientsPopup();
            break;
    }
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
        expandRecipients();
    }
    else
    {
        addRecipientsFromEntry();
        collapseRecipients();
    }

    if(m_pListener)
        m_pListener->onEntryFocusChanged(*this);
}

void ConvRecipientsPanel::onContactButtonClicked()
{
    int currentRecipientsCount = getItemsCount();
    if(currentRecipientsCount < getMaxRecipientCount())
        m_Picker.launch(getMaxRecipientCount() - currentRecipientsCount);
    else
        showTooManyRecipientsNotif();
}

void ConvRecipientsPanel::onContactsPicked(const std::list<int> &numberIdList)
{
    bool duplicateFound = false;
    for(auto phoneNumId : numberIdList)
    {
        ContactPersonNumberRef num = m_App.getContactManager().getContactPersonNumber(phoneNumId);
        if(num)
            duplicateFound |= appendItem(num->getAddress(), num->getDispName(), MsgAddress::UnknownAddressType) == MbeRecipients::DuplicatedStatus;
    }

    if(!duplicateFound)
        setEntryFocus(true);
}

void ConvRecipientsPanel::onPopupBtnClicked(Popup &popup, int buttonId)
{
    popup.destroy();
}

void ConvRecipientsPanel::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    setEntryFocus(true);
}

void ConvRecipientsPanel::showInvalidRecipientsPopup()
{
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.addEventCb(EVAS_CALLBACK_FREE, EVAS_EVENT_CALLBACK(ConvRecipientsPanel, onPopupDel), this);
    popup.setContent(msgt("IDS_MSG_TPOP_UNABLE_TO_ADD_RECIPIENT_NUMBER_NOT_VALID"));
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(ConvRecipientsPanel, onPopupBtnClicked), this);
    popup.show();
}

void ConvRecipientsPanel::showDuplicatedRecipientNotif()
{
    notification_status_message_post(msg("IDS_MSGC_BODY_DUPLICATED_RECIPIENT").cStr());
}

void ConvRecipientsPanel::showTooManyRecipientsNotif()
{
    notification_status_message_post(msgArgs("IDS_MSGC_BODY_MAXIMUM_NUMBER_OF_RECIPIENTS_HPD_REACHED", getMaxRecipientCount()).cStr());
}

void ConvRecipientsPanel::onMbeChanged()
{
    if(!isMbeVisible())
        updateShortenedRecipients();
    if(m_pListener)
        m_pListener->onMbeChanged(*this);
}

void ConvRecipientsPanel::onMbeItemClicked(MbeRecipientItem &item)
{
    if(m_pListener)
        m_pListener->onItemClicked(*this, item);
}

void ConvRecipientsPanel::onContactChanged()
{
    MSG_LOG("");
    if(!getEntryFocus())
        updateShortenedRecipients();
}
