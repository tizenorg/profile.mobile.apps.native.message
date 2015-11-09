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
#include "MsgUtils.h"
#include "Logger.h"
#include "RecipientItem.h"

#include <assert.h>
#include <sstream>
#include <string.h>

using namespace Msg;

void Conversation::createRecipientPanel(Evas_Object *parent)
{
    if(!m_pRecipientPanel)
    {
        m_pRecipientPanel = new RecipientPanel(parent, getMsgEngine().getSettings().getAddressMaxLen());
        m_pRecipientPanel->setListener(this);
        m_pRecipientPanel->setGuideText("Recipients");
        m_pRecipientPanel->show();
    }
}

void Conversation::onEntryFocusChanged(RecipientPanel &obj)
{
    if(obj.getEntryFocus())
    {
        if(m_pRecipientPanel->hasItems())
            m_pRecipientPanel->showMbe(true);
    }
    else
    {
        addRecipientsFromEntry();
        m_pRecipientPanel->showMbe(false);
        clearContactList();
    }
}

void Conversation::clearRecipientEntry()
{
    if(m_pRecipientPanel)
        m_pRecipientPanel->clearEntry();
}

void Conversation::addRecipientsFromEntry()
{
    assert(m_pRecipientPanel);
    if(m_pRecipientPanel)
    {
        std::string text = m_pRecipientPanel->getEntryText();
        auto list = MsgUtils::tokenizeRecipients(text);

        for(auto & it : list)
        {
            addRecipient(it, it);
        }

        m_pRecipientPanel->showMbe(true);
        m_pRecipientPanel->clearEntry();
    }
}

void Conversation::addRecipient(const std::string &address, const std::string &dispName)
{
    assert(m_pRecipientPanel);
    if(m_pRecipientPanel)
    {
        MsgAddress::AddressType addressType = MsgUtils::getAddressType(address);
        if(addressType == MsgAddress::Phone || addressType == MsgAddress::Email)
        {
            RecipientItem *item = new RecipientItem(address, addressType);
            m_pRecipientPanel->appendItem(*item);
        }
        else
        {
            MSG_LOG("invalid recipient: ", address);
        }

        if(m_pRecipientPanel->getEntryFocus())
            m_pRecipientPanel->showMbe(true);
    }
}

void Conversation::onKeyDown(RecipientPanel &obj, Evas_Event_Key_Down *ev)
{
    if(ev && ev->keyname)
    {
        MSG_LOG("Key: ", ev->keyname);

        if((strcmp(ev->keyname, "Return") == 0) || (strcmp(ev->keyname, "semicolon") == 0) || (strcmp(ev->keyname, "comma") == 0))
        {
            m_pBody->setFocus(true);
        }
        else
        {
            updateContactsListRequest();
        }
        /* else if(strcmp(ev->keyname, "BackSpace") == 0)
        {
            // TODO:
        }*/
    }
}

void Conversation::onItemSelected(RecipientPanel &obj, RecipientViewItem &item)
{
    MSG_LOG("");
}

void Conversation::onItemPressed(RecipientPanel &obj, RecipientViewItem &item)
{
    MSG_LOG("");
}

void Conversation::onItemClicked(RecipientPanel &obj, RecipientViewItem &item)
{
    MSG_LOG("");
}

void Conversation::onContactButtonClicked(RecipientPanel &obj)
{
    MSG_LOG("");
}
