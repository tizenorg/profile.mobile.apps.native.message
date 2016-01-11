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

#include "ThreadSearchList.h"
#include "ThreadSearchListItem.h"
#include "MessageSMS.h"
#include "MsgEngine.h"

using namespace Msg;

ThreadSearchList::ThreadSearchList(Evas_Object *parent, App &app)
    : ListView(parent)
    , m_App(app)
    , m_pIdler(nullptr)
    , m_pListener(nullptr)
{
    setMultiSelection(false);
    setMode(ELM_LIST_COMPRESS);
}

ThreadSearchList::~ThreadSearchList()
{
    cancelSearch();
}

void ThreadSearchList::setListener(IThreadSearchListListener *l)
{
    m_pListener = l;
}

void ThreadSearchList::requestSearch(const std::string &searchWord)
{
    m_SearchWord = searchWord;
    if(!m_pIdler)
    {
        m_pIdler = ecore_idler_add
        (
            [](void *data)->Eina_Bool
            {
                ThreadSearchList *self =(ThreadSearchList*)data;
                self->m_pIdler = nullptr;
                self->search();
                return false; // Delete idler
            },
            this
        );
    }
}

void ThreadSearchList::cancelSearch()
{
    if(m_pIdler)
    {
        ecore_idler_del(m_pIdler);
        m_pIdler = nullptr;
    }
}

void ThreadSearchList::search()
{
    ListView::clear();
    // TODO: pull from model:

    // TODO: only for demo will be removed
    MessageSMSRef sms = m_App.getMsgEngine().getComposer().createSms();
    sms->setText("Hello world!!!");
    auto &addr = sms->addAddress();
    addr.setAddress("5555512335555");
    ThreadSearchListItem *item = new ThreadSearchListItem(*sms, m_App, m_SearchWord);
    ListView::appendItem(*item);

    if(m_pListener)
        m_pListener->onSearchListChanged();
}

