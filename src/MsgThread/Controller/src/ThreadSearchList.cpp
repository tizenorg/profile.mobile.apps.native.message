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
#include "MsgSearchListItem.h"
#include "ThreadSearchListItem.h"
#include "MsgEngine.h"
#include "Logger.h"

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

const std::string &ThreadSearchList::getSearchWord() const
{
    return m_SearchWord;
}

void ThreadSearchList::search()
{
    MSG_LOG("Search word: ", m_SearchWord);
    ListView::clear();

    if(!m_SearchWord.empty())
    {
        MsgStorage &msgSotrage = m_App.getMsgEngine().getStorage();

        // Thread:
        MsgThreadListRef threadList = msgSotrage.searchThread(m_SearchWord);
        if(threadList)
        {
            int length = threadList->getLength();
            for(int i = 0; i < length; ++i)
            {
                const MsgThreadItem &threadItem = threadList->at(i);
                ListView::appendItem(*new ThreadSearchListItem(m_App, threadItem, m_SearchWord));
            }
        }

        // Message:
        MessageListRef msgList = msgSotrage.searchMessage(m_SearchWord);
        if(msgList)
        {
            int length = msgList->getLength();
            for(int i = 0; i < length; ++i)
            {
                const Message &msg = msgList->at(i);
                ListView::appendItem(*new MsgSearchListItem(m_App, msg, m_SearchWord));
            }
        }
    }

    if(m_pListener)
        m_pListener->onSearchListChanged();
}

