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
    , m_ComposeCmd()
{
}

Conversation::Conversation(NaviFrameController &parent, const AppControlComposeRef &cmd)
    : Conversation(parent, false)
{
    if(cmd)
    {
        m_ComposeCmd = cmd;
        m_ThreadId = getMsgEngine().getStorage().getThreadId(m_ComposeCmd->getRecipientList());
    }
    create();
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

    getMsgEngine().getStorage().removeListener(*this);
    if(m_pBody)
        m_pBody->setListener(nullptr);
    if(m_pRecipPanel)
        m_pRecipPanel->setListener(nullptr);
    if(m_pContactsList)
        m_pContactsList->setListener(nullptr);
}

void Conversation::create()
{
    createMainLayout(getParent());
    createConvList(*m_pLayout);
    createMsgInputPanel(*m_pLayout);
    createBody(*m_pMsgInputPanel);

    m_pMsgInputPanel->setEntry(*m_pBody);
    updateMsgInputPanel();
    m_pLayout->setMsgInputPanel(*m_pMsgInputPanel);

    setMode(m_ThreadId.isValid() ? ConversationMode : NewMessageMode);

    getMsgEngine().getStorage().addListener(*this);
    setHwButtonListener(*m_pLayout, this);
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
    updateNavibar();
}

void Conversation::setConversationMode()
{
    MSG_LOG("");
    m_Mode = ConversationMode;

    m_pLayout->showPredictSearch(false);
    m_pLayout->showSelectAll(false);
    m_pLayout->setBubble(*m_pConvList);
    m_pConvList->show();
    updateNavibar();

    destroyRecipPanel();
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
    m_pConvList = new ConvList(*m_pLayout, getMsgEngine(), m_ThreadId);
    m_pConvList->hide();
}

void Conversation::createRecipPanel(Evas_Object *parent)
{
    if(!m_pRecipPanel)
    {
        m_pRecipPanel = new RecipientsPanel(parent, getApp(), m_ComposeCmd);
        m_pRecipPanel->setListener(this);
        m_pRecipPanel->show();
        m_pRecipPanel->setRecipientRect(m_pLayout->getRecipientRect());
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
        m_pBody = new Body(*m_pMsgInputPanel, getMsgEngine(), m_ComposeCmd);
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
    if(m_ThreadId.isValid())
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
    }
    else
    {
        // TODO: notify user
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
    getNaviBar().setColor(NaviBar::NaviWhiteColorId);
    std::string conversationName = getMsgEngine().getStorage().getThread(m_ThreadId)->getName();
    if(conversationName.empty())
    {
        getNaviBar().setTitle(msgt("IDS_MSGF_POP_NEW_MESSAGE"));
    }
    else
    {
        //TODO: enable down button when needed
        getNaviBar().showButton(NaviCenterButtonId, true);
        getNaviBar().setButtonText(NaviCenterButtonId, conversationName);
    }
    getNaviBar().showButton(NaviPrevButtonId, true);
}

void Conversation::onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id)
{
    MSG_LOG("MessageInputPanel: button clicked: id = ", id);

    switch(id)
    {
        case MessageInputPanel::AddButtonId:
            // TODO: Only for test, will be removed
            m_pBody->addMedia(PathUtils::getResourcePath(TEST_IMG_PATH));
            break;
        case MessageInputPanel::SendButtonId:
            sendMessage();
            m_pBody->setFocus(true);
            break;
        default:
            break;
    }
}

void Conversation::onContactSelected(ContactListItem &item)
{
    m_pRecipPanel->appendItem(item.getRecipient(), item.getRecipient());
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
        return;
    }

    saveDraftMsg();
    getParent().pop();
}

void Conversation::onHwMoreButtonClicked()
{
    MSG_LOG("");
    //TODO: make more menu popup.
    m_pConvList->setMode(m_pConvList->getMode() == ConvList::NormalMode ? ConvList::SelectMode : ConvList::NormalMode);
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
            break;

        case NaviCenterButtonId:
            break;

        case NaviCancelButtonId:
            break;

        case NaviOkButtonId:
            break;

        default:
            MSG_LOG_ERROR("Invalid buttonId: ", buttonId);
            break;
    }
}
