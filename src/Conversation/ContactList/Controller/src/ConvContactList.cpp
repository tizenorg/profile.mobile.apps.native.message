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

#include "ConvContactList.h"
#include "CallbackAssist.h"
#include "ContactPersonPhoneLog.h"
#include "ContactPersonEmail.h"
#include "ContactPersonPhoneLog.h"

using namespace Msg;

namespace
{
    inline bool isValid(const ContactPersonNumber &rec)
    {
        return rec.isValid() && rec.getNumber();
    }

    inline bool isValid(const ContactPersonEmail &rec)
    {
        return rec.isValid() && rec.getEmail();
    }

    inline bool isValid(const ContactPersonPhoneLog &rec)
    {
        return rec.isValid() && rec.getAddress();
    }
}

ConvContactList::ConvContactList(Evas_Object *parent, ContactManager &cm)
    : ListView(parent)
    , m_pListener(nullptr)
    , m_pPredictSearchIdler(nullptr)
    , m_ContactManager(cm)
{
    ListView::setListener(this);
    ListView::setMultiSelection(true);
    ListView::setMode(ELM_LIST_COMPRESS);
    ListView::setFocusAllow(false);
}

ConvContactList::~ConvContactList()
{
    if(m_pPredictSearchIdler)
    {
        ecore_idler_del(m_pPredictSearchIdler);
        m_pPredictSearchIdler = nullptr;
    }
}

void ConvContactList::setListener(IConvContactListListener *l)
{
    m_pListener = l;
}

void ConvContactList::setSearchWorld(const std::string &searchWord)
{
    m_SearchWord = searchWord;
}

void ConvContactList::requestSearch()
{
    if(!m_pPredictSearchIdler)
        m_pPredictSearchIdler = ecore_idler_add(ECORE_TACK_CALLBACK(ConvContactList, onPredictSearchUpdateRequest), this);
}

void ConvContactList::search()
{
    ListView::clear();
    if(!m_SearchWord.empty())
    {
        search<ContactPersonNumber>();
        search<ContactPersonEmail>();
        search<ContactPersonPhoneLog>();
    }
}

template<typename ContactRecord>
void ConvContactList::search()
{
    auto list = m_ContactManager.search<ContactRecord>(m_SearchWord);
    if(list.isValid())
    {
        do
        {
            auto rec = list.get();
            if(isValid(rec))
            {
                ContactListItem *item = new ContactListItem(rec, m_SearchWord);
                ListView::appendItem(*item);
            }
            else
            {
                MSG_LOG("Skip invalid contact");
            }
        } while(list.next());
         list.release();
     }
}

bool ConvContactList::onPredictSearchUpdateRequest()
{
    m_pPredictSearchIdler = nullptr;
    search();
    return false; // Delete idler
}

void ConvContactList::onListItemSelected(ListItem &listItem, void *funcData)
{
    ContactListItem &contactItem = static_cast<ContactListItem&>(listItem);
    MSG_LOG("Selected recipient: ", contactItem.getRecipient());
    if(m_pListener)
        m_pListener->onContactSelected(contactItem);
}
