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
#include "ContactListItem.h"
#include "App.h"
#include "Logger.h"
#include "ContactManager.h"
#include "CallbackAssist.h"
#include "ContactPersonNumber.h"
#include "ContactPersonEmail.h"

#include <assert.h>
#include <sstream>
#include <string.h>

using namespace Msg;

void Conversation::createPredictSearch(Evas_Object *parent)
{
    m_pContactsList = new ListView(parent);
    m_pContactsList->setListener(this);
    m_pContactsList->expand();
    m_pContactsList->setMultiSelection(true);
    m_pContactsList->show();
    m_pContactsList->setMode(ELM_LIST_COMPRESS);
    m_pContactsList->setFocusAllow(false);

    updateContactsList();
}

void Conversation::updateContactsListRequest()
{
    if(m_pPredictSearchIdler)
        return;

    m_pPredictSearchIdler = ecore_idler_add(ECORE_TACK_CALLBACK(Conversation, onPredictSearchUpdateRequest), this);
    return;
}

void Conversation::updateContactsList()
{
    clearContactList();

    if(!m_pContactsList || !m_pRecipientPanel)
        return;

    ContactManager &cm = getApp().getContactManager();
    std::string searchWord;
    searchWord = m_pRecipientPanel->getEntryText();

    if(!searchWord.empty())
    {
        // TODO: Use template ?

        // ContactPersonNumber:
        {
            auto list = cm.search<ContactPersonNumber>(searchWord);
            if(list.isValid())
            {
                do
                {
                    auto rec = list.get();
                    if(rec.isValid() && rec.getNumber())
                    {
                        ContactListItem *item = new ContactListItem(rec, searchWord);
                        m_pContactsList->appendItem(*item);
                    }
                    else
                    {
                        MSG_LOG_ERROR("Invalid contact");
                    }
                } while(list.next());

                list.release();
            }
        }

        // ContactPersonEmail:
        {
            auto list = cm.search<ContactPersonEmail>(searchWord);
            if(list.isValid())
            {
                do
                {
                    auto rec = list.get();
                    if(rec.isValid() && rec.getEmail())
                    {
                        ContactListItem *item = new ContactListItem(rec, searchWord);
                        m_pContactsList->appendItem(*item);
                    }
                    else
                    {
                        MSG_LOG_ERROR("Invalid contact");
                    }
                } while(list.next());

                list.release();
            }
        }

        // ContactPersonPhoneLog:
        {
            auto list = cm.search<ContactPersonPhoneLog>(searchWord);
            if(list.isValid())
            {
                do
                {
                    auto rec = list.get();
                    if(rec.isValid() && rec.getAddress())
                    {
                        ContactListItem *item = new ContactListItem(rec, searchWord);
                        m_pContactsList->appendItem(*item);
                    }
                    else
                    {
                        MSG_LOG_ERROR("Invalid contact");
                    }
                } while(list.next());

                list.release();
            }
        }
    }
}

void Conversation::clearContactList()
{
    if(m_pContactsList)
        m_pContactsList->clear();
}

bool Conversation::onPredictSearchUpdateRequest()
{
    MSG_LOG("");
    m_pPredictSearchIdler = nullptr;
    updateContactsList();
    return false; // Delete idler
}

void Conversation::onListItemSelected(ListItem &listItem, void *funcData)
{
    MSG_LOG("");
    ContactListItem &contactItem = static_cast<ContactListItem&>(listItem);
    MSG_LOG("Selected recipient: ", contactItem.getRecipient());
    addRecipient(contactItem.getRecipient(), contactItem.getRecipient());
    clearContactList();
    clearRecipientEntry();
}
