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

#include "App.h"
#include "Message.h"
#include "Logger.h"

#include <Elementary.h>
#include <sstream>

using namespace Msg;

void Conversation::createMsgInput(Evas_Object *parent)
{
    if(!m_pMsgInputPanel)
    {
        m_pMsgInputPanel = new MessageInputPanel(parent);
        m_pMsgInputPanel->setListener(this);
        m_pMsgInputPanel->show();
    }

    if(!m_pBody)
    {
        m_pBody = new Body(*m_pMsgInputPanel, getMsgEngine());
        m_pBody->setListener(this);
        m_pBody->show();
        m_pMsgInputPanel->setEntry(*m_pBody);
    }

    updateMsgInputPanel();
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

    MSG_LOG("dis= ", disabledButton);
}
