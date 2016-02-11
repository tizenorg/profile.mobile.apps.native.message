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

using namespace Msg;

ConvRecipientsPanel::ConvRecipientsPanel(Evas_Object *parent, App &app)
    : ConvRecipientsPanelView(parent, app.getMsgEngine().getSettings().getAddressMaxLen())
    , m_App(app)
    , m_pListener(nullptr)
{
    m_Picker.setListener(this);
}

ConvRecipientsPanel::~ConvRecipientsPanel()
{
    m_Picker.setListener(nullptr);
}

void ConvRecipientsPanel::read(Message &msg)
{
    ConvRecipientViewItemList list = getItems();
    for(auto it : list)
    {
        ConvRecipientItem *recipItem = static_cast<ConvRecipientItem*>(it);
        MsgAddress &msgAddr = msg.addAddress();
        msgAddr.setAddress(recipItem->getAddress());
        msgAddr.setRecipientType(recipItem->getRecipientType());
        msgAddr.setAddressType(recipItem->getAddressType());
    }
}

void ConvRecipientsPanel::write(const Message &msg)
{
    update(msg.getAddressList());
}

bool ConvRecipientsPanel::isMms() const
{
    auto items = getItems();
    for(ConvRecipientViewItem *it : items)
    {
        ConvRecipientItem *item = static_cast<ConvRecipientItem*>(it);
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

    for(auto & it : result.validResults)
    {
        if(it.second == MsgAddress::Phone)
            it.first = MsgUtils::makeNormalizedNumber(it.first);

        duplicateFound |= appendItem(it.first, it.second) == DuplicatedStatus;
    }

    if(duplicateFound)
        showDuplicatedRecipientPopup();

    setEntryText(result.invalidResult);
}

void ConvRecipientsPanel::update(const MsgAddressList &addressList)
{
    clearMbe();
    int addrListLen = addressList.getLength();
    for(int i = 0; i < addrListLen; i++)
    {
        auto &addr = addressList.at(i);
        appendItem(addr.getAddress(), addr.getAddressType());
    }
}

void ConvRecipientsPanel::update(const ThreadId &threadId)
{
    clearMbe();
    if(threadId.isValid())
    {
        MsgAddressListRef addrList = m_App.getMsgEngine().getStorage().getAddressList(threadId);
        if(addrList)
            update(*addrList);
    }
}

ConvRecipientsPanel::AppendItemStatus ConvRecipientsPanel::appendItem(const std::string &address, MsgAddress::AddressType addressType)
{
    std::string dispName;
    ContactPersonAddressRef contactAddress = m_App.getContactManager().getContactPersonAddress(address);
    if(contactAddress)
        dispName = contactAddress->getDispName();
    if(dispName.empty())
        dispName = address;

    return appendItem(address, dispName, addressType);
}

ConvRecipientsPanel::AppendItemStatus ConvRecipientsPanel::appendItem(const std::string &address, const std::string &dispName, MsgAddress::AddressType addressType)
{
    AppendItemStatus result = SuccessStatus;
    if(!isRecipientExists(address))
    {
        if(addressType == MsgAddress::UnknownAddressType)
            addressType = MsgUtils::getAddressType(address);

        if(addressType == MsgAddress::Phone || addressType == MsgAddress::Email)
        {
            result = SuccessStatus;
            ConvRecipientItem *item = new ConvRecipientItem(address, addressType);
            item->setDisplayName(dispName);
            ConvRecipientsPanelView::appendItem(*item);
            if(getEntryFocus())
                showMbe(true);
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

void ConvRecipientsPanel::execCmd(const AppControlComposeRef &cmd)
{
    clearMbe();
    for(auto item: cmd->getRecipientList())
    {
        appendItem(item);
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

void ConvRecipientsPanel::onItemAdded(ConvRecipientViewItem &item)
{
    if(m_pListener)
        m_pListener->onItemAdded(*this, static_cast<ConvRecipientItem&>(item));
}

void ConvRecipientsPanel::onItemDeleted(ConvRecipientViewItem &item)
{
    if(m_pListener)
        m_pListener->onItemDeleted(*this, static_cast<ConvRecipientItem&>(item));
}

void ConvRecipientsPanel::onContactButtonClicked()
{
    const int maxRecipientCount = m_App.getMsgEngine().getSettings().getMaxRecipientCount();
    int currentRecipientsCount = getItemsCount();
    if(currentRecipientsCount < maxRecipientCount)
        m_Picker.launch(maxRecipientCount - currentRecipientsCount);
    else
        showTooManyRecipientsPopup();
}

void ConvRecipientsPanel::onItemSelected(ConvRecipientViewItem &item)
{
    MSG_LOG("");
}

void ConvRecipientsPanel::onItemClicked(ConvRecipientViewItem &item)
{
    if(m_pListener)
        m_pListener->onItemClicked(*this, static_cast<ConvRecipientItem&>(item));
}

bool ConvRecipientsPanel::isRecipientExists(const std::string& address) const
{
    ConvRecipientViewItemList recipientList = getItems();
    for(auto pViewItem : recipientList)
    {
        ConvRecipientItem *pItem = dynamic_cast<ConvRecipientItem*>(pViewItem);
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
            duplicateFound |= appendItem(num->getAddress(), num->getDispName(), MsgAddress::Phone) == DuplicatedStatus;
    }

    if(duplicateFound)
        showDuplicatedRecipientPopup();
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

void ConvRecipientsPanel::showDuplicatedRecipientPopup()
{
    auto &popupMngr = m_App.getPopupManager();
    Popup &popup = popupMngr.getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(ConvRecipientsPanel, onPopupDel), this);

    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId,
            POPUP_BUTTON_CB(ConvRecipientsPanel, onPopupBtnClicked), this);

    popup.setContent(msgt("IDS_MSGC_BODY_DUPLICATED_RECIPIENT"));
    popup.show();
}

void ConvRecipientsPanel::showTooManyRecipientsPopup()
{
    auto &popupMngr = m_App.getPopupManager();
    Popup &popup = popupMngr.getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(ConvRecipientsPanel, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId,
            POPUP_BUTTON_CB(ConvRecipientsPanel, onPopupBtnClicked), this);

    int maxRecipientCount = m_App.getMsgEngine().getSettings().getMaxRecipientCount();
    popup.setContent(msgArgs("IDS_MSGC_BODY_MAXIMUM_NUMBER_OF_RECIPIENTS_HPD_REACHED", maxRecipientCount));
    popup.show();
}
