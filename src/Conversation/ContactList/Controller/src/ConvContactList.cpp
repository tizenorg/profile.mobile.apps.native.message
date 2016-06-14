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
    inline bool isValid(const ContactAddress &rec)
    {
        return !rec.getAddress().empty();
    }

    inline bool isValid(const ContactPersonPhoneLog &rec)
    {
        return !rec.getAddress().empty();
    }
}

ConvContactList::ConvContactList(Evas_Object *parent, App &app)
    : ConvContactListView(parent)
    , m_pListener(nullptr)
    , m_pPredictSearchIdler(nullptr)
    , m_App(app)
{
    getList().setListener(this);
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

void ConvContactList::clear()
{
    getList().clear();

    if(m_pListener)
        m_pListener->onContactListChanged();
}

bool ConvContactList::isEmpty() const
{
    return getList().isEmpty();
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
    getList().clear();
    if(!m_SearchWord.empty())
    {
        search<ContactPersonNumber>();
        search<ContactPersonEmail>();
        search<ContactPersonPhoneLog>();
    }
    if(m_pListener)
        m_pListener->onContactListChanged();
}

template<typename ContactRecord>
void ConvContactList::search()
{
    auto list = m_App.getContactManager().search<ContactRecord>(m_SearchWord);
    if(list)
    {
        do
        {
            auto &rec = list->get();
            if(isValid(rec))
            {
                ContactListItem *item = new ContactListItem(rec, m_App, m_SearchWord);
                getList().appendItem(*item);
            }
            else
            {
                MSG_LOG("Skip invalid contact");
            }
        } while(list->next());
     }
}

bool ConvContactList::onPredictSearchUpdateRequest()
{
    m_pPredictSearchIdler = nullptr;
    search();
    return false; // Delete idler
}

void ConvContactList::onListItemSelected(ListItem &listItem)
{
    ContactListItem &contactItem = static_cast<ContactListItem&>(listItem);
    MSG_LOG("Selected recipient: ", contactItem.getRecipient());
    if(m_pListener)
        m_pListener->onContactSelected(contactItem);
}
