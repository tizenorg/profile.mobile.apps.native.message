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
    , m_pContactsList(nullptr)
    , m_pMsgInputPanel(nullptr)
    , m_pBody(nullptr)
    , m_pRecipientPanel(nullptr)
    , m_ThreadId()
    , m_pPredictSearchIdler(nullptr)
{
    create(NewMessageMode);
}

Conversation::Conversation(NaviFrameController &parent,ThreadId threadId)
    : FrameController(parent)
    , m_Mode(InitMode)
    , m_pLayout(nullptr)
    , m_pScroller(nullptr)
    , m_pBubbleBox(nullptr)
    , m_pContactsList(nullptr)
    , m_pMsgInputPanel(nullptr)
    , m_pBody(nullptr)
    , m_pRecipientPanel(nullptr)
    , m_ThreadId(threadId)
    , m_pPredictSearchIdler(nullptr)

{
    create(ConversationMode);
}

Conversation::~Conversation()
{
    getMsgEngine().getStorage().removeListener(*this);
    if(m_pPredictSearchIdler)
    {
        ecore_idler_del(m_pPredictSearchIdler);
        m_pPredictSearchIdler = nullptr;
    }
}

void Conversation::create(Mode mode)
{
    m_pLayout = new ConversationLayout(getParent().getEvasObject());
    m_pLayout->show();
    m_pLayout->expand();

    m_pScroller = new Scroller(*m_pLayout);
    m_pScroller->show();
    m_pScroller->expand();
    m_pScroller->setPpolicy(ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);

    createMsgInput(*m_pLayout);

    m_pLayout->setBubble(*m_pScroller);
    m_pLayout->setMsgInputPanel(*m_pMsgInputPanel);

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

    createRecipientPanel(*m_pLayout);
    m_pRecipientPanel->setRecipientRect(m_pLayout->getRecipientRect());

    createPredictSearch(*m_pLayout);

    m_pLayout->setRecipientPanel(*m_pRecipientPanel);
    m_pLayout->setPredictSearch(*m_pContactsList);
}

void Conversation::setConversationMode()
{
    m_Mode = ConversationMode;

    m_pLayout->showPredictSearch(false);
    m_pLayout->showSelectAll(false);

    if(m_pRecipientPanel)
    {
        m_pRecipientPanel->destroy();
        m_pRecipientPanel = nullptr;
    }

    createBubbleList(*m_pScroller);
    m_pBubbleBox->setSizeHintAlign(EVAS_HINT_FILL, EVAS_HINT_FILL);
    m_pBubbleBox->setSizeHintWeight(EVAS_HINT_EXPAND, 0);
    m_pScroller->setContent(*m_pBubbleBox);
}

void Conversation::fillMessage(Message &msg)
{
    if(MessageSMS *sms = dynamic_cast<MessageSMS*>(&msg))
    {
        fillMsgBody(*sms);
    }

    fillMsgAddress(msg);
}

void Conversation::fillMsgAddress(Message &msg)
{
    if(m_Mode == ConversationMode)
    {
        MsgAddressListRef addressList = getMsgEngine().getStorage().getAddressList(m_ThreadId);
        msg.addAddresses(*addressList);
    }
    else if(m_Mode == NewMessageMode)
    {
        assert(m_pRecipientPanel);
        if(m_pRecipientPanel)
        {
            RecipientViewItemList list = m_pRecipientPanel->getItems();
            for(auto &it : list)
            {
                RecipientItem *recipItem = static_cast<RecipientItem*>(it);
                MsgAddress &msgAddr = msg.addAddress();
                msgAddr.setAddress(recipItem->getAddress());
                msgAddr.setRecipientType(recipItem->getRecipientType());
                msgAddr.setAddressType(recipItem->getAddressType());
            }
        }
    }
}

void Conversation::fillMsgBody(MessageSMS &msg)
{
    // TODO: mms + attachments
    msg.setText(m_pBody->getText());
}

void Conversation::sendMessage()
{
    MessageSMSRef msg = getMsgEngine().getStorage().createSms();
    fillMessage(*msg);
    MSG_LOG("m_ThreadId = ", m_ThreadId);
    getMsgEngine().getTransport().sendMessage(msg, &m_ThreadId);
    MSG_LOG("m_ThreadId = ", m_ThreadId);

    if(m_Mode == NewMessageMode)
    {
        setMode(ConversationMode);
    }
}

void Conversation::onMsgStorageChange()
{
    MSG_LOG("");

    fillConversationList();
    m_pScroller->navigateToBottom();
}

void Conversation::onViewItemCreated()
{
    getNaviBar().setTitle("Conversation");
    setHwButtonListener(getContent(), this);
}

Evas_Object *Conversation::getContent()
{
    return *m_pLayout;
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

