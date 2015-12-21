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
#include "ResourceUtils.h"
#include "Scroller.h"
#include "App.h"
#include "Message.h"
#include "MessageSMS.h"
#include "Logger.h"
#include "Box.h"
#include "RecipientItem.h"

#include <Elementary.h>
#include <sstream>

using namespace Msg;

Conversation::Conversation(NaviFrameController &parent)
    : FrameController(parent)
    , m_Mode(InitMode)
    , m_pLayout(nullptr)
    , m_pScroller(nullptr)
    , m_pBubbleBox(nullptr)
    , m_pMsgInputPanel(nullptr)
    , m_pBody(nullptr)
    , m_pRecipPanel(nullptr)
    , m_pContactsList(nullptr)
    , m_ThreadId()
{
    create(NewMessageMode);
}

Conversation::Conversation(NaviFrameController &parent,ThreadId threadId)
    : Conversation(parent)
{
    m_ThreadId = threadId;
    create(ConversationMode);
}

Conversation::~Conversation()
{
    saveDraftMsg();
    getMsgEngine().getStorage().removeListener(*this);
}

void Conversation::create(Mode mode)
{
    createMainLayout(getParent());

    m_pScroller = new Scroller(*m_pLayout);
    m_pScroller->show();
    m_pScroller->expand();
    m_pScroller->setPpolicy(ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);

    createMsgInputPanel(*m_pLayout);
    createBody(*m_pMsgInputPanel);
    m_pMsgInputPanel->setEntry(*m_pBody);
    updateMsgInputPanel();

    m_pLayout->setMsgInputPanel(*m_pMsgInputPanel);
    m_pLayout->setBubble(*m_pScroller);

    setMode(mode);
    fillConversationList();

    getMsgEngine().getStorage().addListener(*this);
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

    m_pLayout->setRecipientPanel(*m_pRecipPanel);
    m_pLayout->setPredictSearch(*m_pContactsList);
}

void Conversation::setConversationMode()
{
    m_Mode = ConversationMode;

    m_pLayout->showPredictSearch(false);
    m_pLayout->showSelectAll(false);

    destroyRecipPanel();
    destroyContactList();
    createBubbleList(*m_pScroller);

    m_pBubbleBox->setSizeHintAlign(EVAS_HINT_FILL, EVAS_HINT_FILL);
    m_pBubbleBox->setSizeHintWeight(EVAS_HINT_EXPAND, 0);
    m_pScroller->setContent(*m_pBubbleBox);
}

void Conversation::createMainLayout(Evas_Object *parent)
{
    m_pLayout = new ConversationLayout(parent);
    m_pLayout->show();
    m_pLayout->expand();
}

void Conversation::createRecipPanel(Evas_Object *parent)
{
    if(!m_pRecipPanel)
    {
        m_pRecipPanel = new RecipientsPanel(parent, getApp());
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
    MsgTransport::ReturnType sendRes = getMsgEngine().getTransport().sendMessage(*msg, &m_ThreadId);

    // TODO: handle send result

    MSG_LOG("Send result = ", sendRes);
    MSG_LOG("m_ThreadId = ", m_ThreadId);

    if(m_Mode == NewMessageMode)
    {
        setMode(ConversationMode);
    }
}

void Conversation::saveDraftMsg()
{
    if(m_pBody && !m_pBody->isEmpty())
    {
        // TODO: impl. for mms draft
        auto msg = getMsgEngine().getComposer().createSms();

        fillMessage(*msg);
        MsgId msgId = getMsgEngine().getStorage().saveMessage(*msg);
        MSG_LOG("Draft message id = ", msgId);
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

void Conversation::onChanged(Body &body)
{
    updateMsgInputPanel();
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

void Conversation::onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id)
{
    MSG_LOG("MessageInputPanel: button clicked: id = ", id);

    switch(id)
    {
        case MessageInputPanel::AddButtonId:
            break;
        case MessageInputPanel::SendButtonId:
            sendMessage();
            m_pBody->clear();
            m_pBody->setFocus(true);
            break;
        default:
            break;
    }
}

void Conversation::onEntryFocusChanged(RecipientsPanel &panel)
{
    if(!m_pRecipPanel->getEntryFocus())
        m_pContactsList->clear();
}

void Conversation::onMsgStorageChange(const MsgIdList &idList)
{
    MSG_LOG("");

    fillConversationList();
    m_pScroller->navigateToBottom();
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
    setHwButtonListener(*m_pLayout, this);
    setContent(*m_pLayout);
}

void Conversation::onHwBackButtonClicked()
{
    MSG_LOG("");
    getParent().pop();
}

void Conversation::onHwMoreButtonClicked()
{
    MSG_LOG("");
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

