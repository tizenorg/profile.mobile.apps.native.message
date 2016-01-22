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

#include "Conversation.h"
#include "Logger.h"
#include "PathUtils.h"
#include "Scroller.h"
#include "App.h"
#include "Message.h"
#include "Logger.h"
#include "RecipientItem.h"
#include "LangUtils.h"
#include "CallbackAssist.h"

#include <Elementary.h>
#include <sstream>
#include <notification.h>

using namespace Msg;

Conversation::Conversation(NaviFrameController &parent, bool dummy)
    : FrameController(parent)
    , m_Mode(InitMode)
    , m_pLayout(nullptr)
    , m_pMsgInputPanel(nullptr)
    , m_pBody(nullptr)
    , m_pRecipPanel(nullptr)
    , m_pContactsList(nullptr)
    , m_ThreadId()
    , m_IsMms(false)
    , m_pConvList(nullptr)
    , m_AttachPanel(getApp())
{
}

Conversation::Conversation(NaviFrameController &parent, const AppControlComposeRef &cmd)
    : Conversation(parent, false)
{
    if(cmd)
        m_ThreadId = getMsgEngine().getStorage().getThreadId(cmd->getRecipientList());

    create();
    execCmd(cmd);
}

Conversation::Conversation(NaviFrameController &parent, const AppControlDefaultRef &cmd)
    : Conversation(parent, false)
{
    if(cmd)
        m_ThreadId = getMsgEngine().getStorage().getMessage(cmd->getMessageId())->getThreadId();

    create();

    if(cmd->getDefaultType() == AppControlDefault::ReplyType)
        m_pBody->setFocus(true);

}

Conversation::Conversation(NaviFrameController &parent,ThreadId threadId)
    : Conversation(parent, false)
{
    m_ThreadId = threadId;
    create();
}

Conversation::~Conversation()
{
    // Call before delete all children:
    MSG_LOG("");
    saveDraftMsg();
    getMsgEngine().getStorage().removeListener(*this);
    if(m_pBody)
        m_pBody->setListener(nullptr);
    if(m_pRecipPanel)
        m_pRecipPanel->setListener(nullptr);
    if(m_pContactsList)
        m_pContactsList->setListener(nullptr);
    m_AttachPanel.setListener(nullptr);
}

void Conversation::execCmd(const AppControlComposeRef &cmd)
{
    if(cmd)
    {
        if(m_pRecipPanel)
            m_pRecipPanel->execCmd(cmd);
        if(m_pBody)
            m_pBody->execCmd(cmd);
    }
}

void Conversation::create()
{
    markAsRead();

    createMainLayout(getParent());
    createMsgInputPanel(*m_pLayout);
    createBody(*m_pMsgInputPanel);

    m_pMsgInputPanel->setEntry(*m_pBody);
    updateMsgInputPanel();
    m_pLayout->setMsgInputPanel(*m_pMsgInputPanel);

    setMode(m_ThreadId.isValid() ? ConversationMode : NewMessageMode);

    getMsgEngine().getStorage().addListener(*this);
    setHwButtonListener(*m_pLayout, this);
    m_AttachPanel.setListener(this);
}

void Conversation::markAsRead()
{
    if(m_ThreadId.isValid())
        getMsgEngine().getStorage().setReadStatus(m_ThreadId);
}

void Conversation::navigateTo(MsgId msgId)
{
    m_pConvList->navigateTo(msgId);
}

void Conversation::setMode(Mode mode)
{
    if(m_Mode == mode)
        return;

    switch(mode)
    {
        case NewMessageMode:
            if(m_Mode == InitMode)
            {
                setNewMessageMode();
            }
            else
            {
                MSG_LOG_ERROR("Wrong mode");
            }
            break;

        case ConversationMode:
            setConversationMode();
            break;

        case InitMode:
            break;

        default:
            MSG_LOG_ERROR("Wrong mode");
            break;
    }
}

void Conversation::setNewMessageMode()
{
    m_Mode = NewMessageMode;

    createRecipPanel(*m_pLayout);
    createContactList(*m_pLayout);

    m_pLayout->showPredictSearch(true);
    m_pLayout->setRecipientPanel(*m_pRecipPanel);
    m_pLayout->setPredictSearch(*m_pContactsList);
    destroyConvList();
    updateNavibar();
    m_pRecipPanel->update(m_ThreadId);
    m_pRecipPanel->showMbe(!m_pRecipPanel->isMbeEmpty());
}

void Conversation::setConversationMode()
{
    MSG_LOG("");

    m_Mode = ConversationMode;
    createConvList(*m_pLayout);

    m_pLayout->showPredictSearch(false);
    m_pLayout->showSelectAll(false);
    m_pLayout->setBubble(*m_pConvList);

    updateNavibar();

    MsgAddressListRef addressList = getMsgEngine().getStorage().getAddressList(m_ThreadId);
    if(addressList && addressList->getLength() > 1)
    {
        createRecipPanel(*m_pLayout);
        m_pLayout->setRecipientPanel(*m_pRecipPanel);
        m_pRecipPanel->showEntry(false);
        m_pRecipPanel->update(addressList);
    }
    else
    {
        destroyRecipPanel();
    }
    destroyContactList();
}

void Conversation::createMainLayout(Evas_Object *parent)
{
    m_pLayout = new ConversationLayout(parent);
    m_pLayout->show();
    m_pLayout->expand();
}

void Conversation::createConvList(Evas_Object *parent)
{
    if(!m_pConvList)
    {
        m_pConvList = new ConvList(*m_pLayout, m_ThreadId, getApp());
        m_pConvList->setListener(this);
        m_pConvList->show();
    }
}

void Conversation::destroyConvList()
{
    if(m_pConvList)
    {
        m_pConvList->destroy();
        m_pConvList = nullptr;
    }
}

void Conversation::createRecipPanel(Evas_Object *parent)
{
    if(!m_pRecipPanel)
    {
        m_pRecipPanel = new RecipientsPanel(parent, getApp());
        m_pRecipPanel->setListener(this);
        m_pRecipPanel->show();
        m_pLayout->setRecipientPanel(m_pRecipPanel->getAreaRect());
    }
}

void Conversation::destroyRecipPanel()
{
    if(m_pRecipPanel)
    {
        m_pRecipPanel->destroy();
        m_pRecipPanel = nullptr;
    }
}

void Conversation::createContactList(Evas_Object *parent)
{
    if(!m_pContactsList)
    {
        m_pContactsList = new ConvContactList(parent, getApp().getContactManager());
        m_pContactsList->setListener(this);
        m_pContactsList->show();
    }
}

void Conversation::destroyContactList()
{
    if(m_pContactsList)
    {
        m_pContactsList->destroy();
        m_pContactsList = nullptr;
    }
}

void Conversation::createMsgInputPanel(Evas_Object *parent)
{
    if(!m_pMsgInputPanel)
    {
        m_pMsgInputPanel = new MessageInputPanel(parent);
        m_pMsgInputPanel->setListener(this);
        m_pMsgInputPanel->show();
    }
}

void Conversation::createBody(Evas_Object *parent)
{
    if(!m_pBody)
    {
        m_pBody = new Body(*m_pMsgInputPanel, getMsgEngine());
        m_pBody->setListener(this);
        m_pBody->show();
    }
}

void Conversation::fillMessage(Message &msg)
{
    m_pBody->read(msg);
    fillMsgAddress(msg);
}

void Conversation::fillMsgAddress(Message &msg)
{
    if(m_ThreadId.isValid() && m_Mode != NewMessageMode)
    {
        MsgAddressListRef addressList = getMsgEngine().getStorage().getAddressList(m_ThreadId);
        msg.addAddresses(*addressList);
    }
    else
    {
        if(m_pRecipPanel)
            m_pRecipPanel->read(msg);
    }
}

void Conversation::sendMessage()
{
    if(!m_ThreadId.isValid() && m_pRecipPanel->isMbeEmpty())
    {
        showNoRecipPopup();
        return;
    }

    auto msg = getMsgEngine().getComposer().createSms();
    fillMessage(*msg);
    MSG_LOG("m_ThreadId = ", m_ThreadId);
    MsgTransport::SendResult sendRes = getMsgEngine().getTransport().sendMessage(*msg, &m_ThreadId);

    MSG_LOG("Send result = ", sendRes);
    MSG_LOG("m_ThreadId = ", m_ThreadId);

    if(sendRes == MsgTransport::SendSuccess && m_ThreadId.isValid())
    {
        if(m_Mode == NewMessageMode)
            setMode(ConversationMode);

        m_pConvList->setThreadId(m_ThreadId);
        m_IsMms = false;
        m_pBody->clear();
        m_pBody->setFocus(true);
    }
    else
    {
        showSendResultPopup(sendRes);
    }
}

void Conversation::saveDraftMsg()
{
    if(m_pBody && !m_pBody->isEmpty())
    {
        MessageRef msg = getMsgEngine().getComposer().createMessage(m_IsMms ? Message::MT_MMS : Message::MT_SMS);

        if(msg)
        {
            fillMessage(*msg);
            MsgId msgId = getMsgEngine().getStorage().saveMessage(*msg);
            MSG_LOG("Draft message id = ", msgId);
        }
    }
}

void Conversation::notifyConvertMsgType()
{
    std::string notifText = m_IsMms ?
            msg("IDS_MSGF_POP_CONVERTING_TO_MULTIMEDIA_MESSAGE_ING") :
            (std::string)msg("IDS_MSGF_POP_CONVERTING_TO_TEXT_MESSAGE_ING");

    notification_status_message_post(notifText.c_str());
}

void Conversation::convertMsgTypeHandler()
{
    MSG_LOG("Is MMS: ", m_IsMms);
    notifyConvertMsgType();
}

void Conversation::checkAndSetMsgType()
{
    // Body:
    bool isMms = !m_pBody->isEmpty() && m_pBody->isMms();

    // Recipients:
    if(!isMms && m_pRecipPanel && !m_pRecipPanel->isMbeEmpty())
        isMms = m_pRecipPanel->isMms();

    if(isMms != m_IsMms)
    {
        m_IsMms = isMms;
        convertMsgTypeHandler();
    }
}

void Conversation::hideKeyboard()
{
}

void Conversation::showNoRecipPopup()
{
    auto &popupMngr = getApp().getPopupManager();
    Popup &popup = popupMngr.getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Conversation, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(Conversation, onMsgSendErrorButtonClicked), this);
    popup.setContent(msgt("IDS_MSG_POP_YOUR_MESSAGE_WILL_BE_DISCARDED_NO_RECIPIENTS_HAVE_BEEN_SELECTED"));
    popup.show();
}

void Conversation::showSendResultPopup(MsgTransport::SendResult result)
{
    if(result == MsgTransport::SendSuccess)
        return;

    const char *strId = nullptr;
    switch(result)
    {
        case MsgTransport::SendNoSIM:
            strId = "IDS_MSG_BODY_UNABLE_TO_SEND_THIS_MESSAGE_INSERT_YOUR_SIM_CARD_AND_TRY_AGAIN";
            break;
        default:
            strId = "IDS_MSGC_BODY_UNABLE_TO_SEND_MESSAGE";
    };

    auto &popupMngr = getApp().getPopupManager();
    Popup &popup = popupMngr.getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Conversation, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(Conversation, onMsgSendErrorButtonClicked), this);
    popup.setContent(msgt(strId));
    popup.show();
}

void Conversation::showMainCtxPopup()
{
    auto &ctxPopup = getApp().getPopupManager().getCtxPopup();

    ctxPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(Conversation, onDeleteItemPressed), this);
    ctxPopup.appendItem(msg("IDS_MSG_OPT_ADD_RECIPIENTS_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(Conversation, onAddRecipientsItemPressed), this);
    ctxPopup.align(getApp().getWindow());
    ctxPopup.show();
}

void Conversation::onKeyDown(RecipientsPanel &panel, Evas_Event_Key_Down &ev)
{
    if(ev.keyname)
    {
        if((strcmp(ev.keyname, "Return") == 0))
        {
            m_pBody->setFocus(true);
        }
        else
        {
            m_pContactsList->setSearchWorld(m_pRecipPanel->getEntryText());
            m_pContactsList->requestSearch();
        }
    }
}

void Conversation::onEntryFocusChanged(RecipientsPanel &panel)
{
    if(!m_pRecipPanel->getEntryFocus())
        m_pContactsList->clear();
}

void Conversation::onItemAdded(RecipientsPanel &panel, RecipientItem &item)
{
    checkAndSetMsgType();
}

void Conversation::onItemDeleted(RecipientsPanel &panel, RecipientItem &item)
{
    checkAndSetMsgType();
}

void Conversation::onChanged(Body &body)
{
    updateMsgInputPanel();
    checkAndSetMsgType();
}

void Conversation::updateMsgInputPanel()
{
    // TODO:
    BodySmsSize size = m_pBody->getSmsSize();

    std::stringstream ss;
    ss << size.charsLeft << "/" << size.smsCount;
    m_pMsgInputPanel->setCounter(ss.str());

    bool disabledButton = m_pBody->isEmpty();
    m_pMsgInputPanel->disabledButton(MessageInputPanel::SendButtonId, disabledButton);
}

void Conversation::updateNavibar()
{
    getNaviBar().clear();
    getNaviBar().setColor(NaviBar::NaviWhiteColorId);

    if(m_Mode == NewMessageMode)
    {
        getNaviBar().setTitle(msgt("IDS_MSGF_POP_NEW_MESSAGE"));
        getNaviBar().showButton(NaviPrevButtonId, true);
    }
    else
    {
        if(m_pConvList->getMode() == ConvList::SelectMode)
        {
            getNaviBar().setTitle(msgt("IDS_MSG_OPT_DELETE"));
            getNaviBar().showButton(NaviCancelButtonId, true);
            getNaviBar().showButton(NaviOkButtonId, true);
        }
        else
        {
            MsgAddressListRef addressList = getMsgEngine().getStorage().getAddressList(m_ThreadId);
            if(addressList && !addressList->isEmpty())
            {
                std::string conversationName;
                std::string firstNumber = addressList->at(0).getAddress();
                ContactPersonNumber contactPersonNumber = getApp().getContactManager().getContactPersonNumber(firstNumber);
                if(contactPersonNumber.isValid())
                {
                    const char *dispName = contactPersonNumber.getDispName();
                    if(dispName)
                        conversationName = dispName;
                    contactPersonNumber.release();
                }

                if(conversationName.empty())
                    conversationName = firstNumber;

                int hidenAddresses= addressList->getLength() - 1;
                if(hidenAddresses > 0)
                {
                    conversationName += " + " + std::to_string(hidenAddresses);
                    getNaviBar().showButton(NaviDownButtonId, true);
                }

                getNaviBar().showButton(NaviCenterButtonId, true);
                getNaviBar().showButton(NaviPrevButtonId, true);
                getNaviBar().setButtonText(NaviCenterButtonId, conversationName);
            }
        }
    }
}

void Conversation::onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id)
{
    MSG_LOG("MessageInputPanel: button clicked: id = ", id);

    switch(id)
    {
        case MessageInputPanel::AddButtonId:
            m_AttachPanel.show(true);
            break;
        case MessageInputPanel::SendButtonId:
            sendMessage();
            break;
        default:
            break;
    }
}

void Conversation::onContactSelected(ContactListItem &item)
{
    m_pRecipPanel->appendItem(item.getRecipient());
    m_pRecipPanel->clearEntry();
    m_pContactsList->clear();
}

void Conversation::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    updateNavibar();
    setContent(*m_pLayout);
}

void Conversation::onHwBackButtonClicked()
{
    MSG_LOG("");
    if(m_pConvList && m_pConvList->getMode() == ConvList::SelectMode)
    {
        m_pConvList->setMode(ConvList::NormalMode);
        updateNavibar();
        return;
    }

    getParent().pop();
}

void Conversation::onHwMoreButtonClicked()
{
    MSG_LOG("");
    if(m_Mode == ConversationMode && m_pConvList->getMode() == ConvList::NormalMode)
        showMainCtxPopup();
}

void Conversation::onNaviOkButtonClicked()
{
    MSG_LOG("");
    if(m_Mode == ConversationMode && m_pConvList->getMode() == ConvList::SelectMode)
    {
        m_pConvList->deleteSelectedItems();
        m_pConvList->setMode(ConvList::NormalMode);
        updateNavibar();
    }
}

void Conversation::onNaviCenterButtonClicked()
{
    MSG_LOG("");
}

void Conversation::onNaviDownButtonClicked()
{
    MSG_LOG("");
    if(m_pRecipPanel)
    {
        bool isMbeInvisible= !m_pRecipPanel->isMbeVisible();
        m_pRecipPanel->showMbe(isMbeInvisible);
        getNaviBar().setDownButtonState(isMbeInvisible);
    }
}

void Conversation::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    //TODO: Handle other buttons
    switch(buttonId)
    {
        case NaviPrevButtonId:
            onHwBackButtonClicked();
            break;

        case NaviDownButtonId:
            onNaviDownButtonClicked();
            break;

        case NaviCenterButtonId:
            onNaviCenterButtonClicked();
            break;

        case NaviCancelButtonId:
            onHwBackButtonClicked();
            break;

        case NaviOkButtonId:
            onNaviOkButtonClicked();
            break;

        default:
            MSG_LOG_ERROR("Invalid buttonId: ", buttonId);
            break;
    }
}

void Conversation::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    m_pBody->setFocus(true);
}

void Conversation::onMsgSendErrorButtonClicked(Popup &popup, int buttonId)
{
    m_pBody->setFocus(true);
    popup.destroy();
}

void Conversation::onDeleteItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();

    m_pConvList->setMode(ConvList::SelectMode);
    setConversationMode();
}

void Conversation::onAddRecipientsItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();
    setNewMessageMode();
}

void Conversation::onAllItemsDeleted(ConvList &list)
{
    onHwBackButtonClicked();
}

void Conversation::onFileSelected(AttachPanel &panel, const AttachPanel::FileList &files)
{
    m_pBody->addMedia(files);
}
