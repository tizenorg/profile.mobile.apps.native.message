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
#include "ContactManager.h"
#include "CallbackAssist.h"

using namespace Msg;

#define DEF_BUF_LEN_L   (4096)

RecipientsPanel::RecipientsPanel(Evas_Object *parent, App &app)
    : RecipientsPanelView(parent, app.getMsgEngine().getSettings().getAddressMaxLen())
    , m_App(app)
    , m_pListener(nullptr)
{
}

RecipientsPanel::RecipientsPanel(Evas_Object *parent, App &app, const AppControlComposeRef &cmd)
    : RecipientsPanel(parent, app)
{
    if(cmd)
        execCmd(cmd);
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
        m_Recipients[address] = item;

        if(getEntryFocus())
            showMbe(true);
    }
    else
    {
        MSG_LOG("invalid recipient: ", address);
    }
    return result;
}

void RecipientsPanel::execCmd(const AppControlComposeRef &cmd)
{
    for(auto item: cmd->getRecipientList())
    {
        appendItem(item, item, MsgAddress::UnknownAddressType);
    }
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
    RecipientItem &recItem = static_cast<RecipientItem&>(item);
    auto foundIt = m_Recipients.find(recItem.getAddress());
    if (foundIt != m_Recipients.end())
        m_Recipients.erase(foundIt);

    if(m_pListener)
        m_pListener->onItemDeleted(*this, recItem);
}

void RecipientsPanel::onContactButtonClicked()
{
    app_control_h svc_handle = NULL;
    if(APP_CONTROL_ERROR_NONE == app_control_create(&svc_handle))
    {
        const int maxRecipientCount = m_App.getMsgEngine().getSettings().getMaxRecipientCount();
        int currentRecipientsCount = getMbeItemsCount();
        if(currentRecipientsCount < maxRecipientCount)
        {
            app_control_set_operation(svc_handle, APP_CONTROL_OPERATION_PICK);
            app_control_set_mime(svc_handle, APP_CONTROL_MIME_CONTACT);
            if(maxRecipientCount - currentRecipientsCount > 1)
            {
                app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_SELECTION_MODE, APP_CONTROL_DATA_SELECTION_MODE_MULTIPLE);
                app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_TOTAL_COUNT,
                        std::to_string(maxRecipientCount - currentRecipientsCount).c_str());
            }
            else
            {
                app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_SELECTION_MODE, APP_CONTROL_DATA_SELECTION_MODE_SINGLE);
            }

            //TODO: We have to request a possibility to acquire phone numbers and emails at the same time
            app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_RESULT_TYPE, APP_CONTROL_DATA_RESULT_TYPE_PHONE);
            int ret = app_control_send_launch_request(svc_handle, APP_CONTROL_RES_CALLBACK(RecipientsPanel, onAppControlRes), this);
            if(ret != APP_CONTROL_ERROR_NONE)
            {
                MSG_LOG("failed to launch contacts, error-code: ", ret);
                app_control_destroy(svc_handle);
                return;
            }
        }
        else
        {
            auto &popupMngr = m_App.getPopupManager();
            Popup &popup = popupMngr.getPopup();
            popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(RecipientsPanel, onPopupDel), this);
            popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId,
                    POPUP_BUTTON_CB(RecipientsPanel, onPopupBtnClicked), this);

            popup.setContent(msgArgs("IDS_MSGC_BODY_MAXIMUM_NUMBER_OF_RECIPIENTS_HPD_REACHED", maxRecipientCount));
            popup.show();
        }
    }
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

void RecipientsPanel::onAppControlRes(app_control_h request, app_control_h reply, app_control_result_e result)
{
    if(result == APP_CONTROL_RESULT_SUCCEEDED)
    {
        std::list<int> phoneNumIds;
        AppControlUtils::getExtraDataIntArray(reply, APP_CONTROL_DATA_SELECTED, phoneNumIds);

        unsigned int appended = 0;
        std::string addr;
        for(auto phone : phoneNumIds)
        {
            ContactPersonNumber num = m_App.getContactManager().getContactPersonByPhoneId(phone);
            addr = num.getNumber();
            bool shouldAdd = true;
            if (!m_Recipients.empty())
            {
                if (m_Recipients.find(addr) !=  m_Recipients.end())
                    shouldAdd = false;
            }

            if (shouldAdd)
            {
                appendItem(addr, num.getDispName(), MsgAddress::Phone);
                ++appended;
            }
        }

        if(phoneNumIds.size() > appended)
        {
            auto &popupMngr = m_App.getPopupManager();
            Popup &popup = popupMngr.getPopup();
            popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(RecipientsPanel, onPopupDel), this);

            popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId,
                    POPUP_BUTTON_CB(RecipientsPanel, onPopupBtnClicked), this);

            popup.setContent(msgt("IDS_MSGC_BODY_DUPLICATED_RECIPIENT"));
            popup.show();
        }
        else
        {
            setEntryFocus(true);
        }
    }
}


void RecipientsPanel::onPopupBtnClicked(Popup &popup, int buttonId)
{
    setEntryFocus(true);
    popup.destroy();
}

void RecipientsPanel::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    setEntryFocus(true);
}
