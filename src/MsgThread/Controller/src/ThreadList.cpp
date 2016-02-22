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


#include "ThreadList.h"
#include "ThreadListItem.h"
#include "ListItem.h"
#include "SelectAllListItem.h"
#include "App.h"

using namespace Msg;

namespace
{
    inline bool isSelectAll(ListItem *it)
    {
        return dynamic_cast<SelectAllListItem*>(it) != nullptr;
    }
}

ThreadList::ThreadList(Evas_Object *parent, App &app)
    : ListView(parent)
    , m_pListener(nullptr)
    , m_App(app)
    , m_DeleteMode(false)
{
    ListView::setListener(this);
    ListView::setMultiSelection(false);
    ListView::setMode(ELM_LIST_COMPRESS);
    m_App.getMsgEngine().getStorage().addListener(*this);
    m_App.getContactManager().addListener(*this);
    updateList();
}

ThreadList::~ThreadList()
{
    m_App.getMsgEngine().getStorage().removeListener(*this);
    m_App.getContactManager().removeListener(*this);
}

void ThreadList::setListener(IThreadListListener *l)
{
    m_pListener = l;
}

void ThreadList::setDeleteMode(bool value)
{
    m_DeleteMode = value;
    setCheckMode(value);
    checkAllItems(false);
    showSelectAllItem(value);
}

bool ThreadList::isDeleteModeEnabled() const
{
    return m_DeleteMode;
}

void ThreadList::deleteSelectedItems()
{
    auto collection = getItems<ThreadListItem>();
    for(ThreadListItem *it : collection)
    {
        if(it->getCheckedState())
            m_App.getMsgEngine().getStorage().deleteThread(it->getThreadId());
    }
}

int ThreadList::getThreadsCheckedCount() const
{
    auto items = getItems<ThreadListItem>();
    int count = 0;
    for(ThreadListItem *item : items)
    {
        if(item->isCheckable() && item->getCheckedState())
            count++;
    }
    return count;
}

void ThreadList::showSelectAllItem(bool show, bool resetCheck)
{
    SelectAllListItem *item = dynamic_cast<SelectAllListItem*>(getFirstItem());

    if(show)
    {
        if(!item)
        {
            item = new SelectAllListItem;
            prependItem(*item);
        }

        if(item && resetCheck)
            item->setCheckedState(false, true);
    }
    else
    {
        if(item)
            item->destroy();
    }
}

void ThreadList::checkAllItems(bool check)
{
    auto items = getItems();
    for(ListItem *it : items)
    {
        if(it->isCheckable() && !isSelectAll(it))
            it->setCheckedState(check, true);
    }
}

bool ThreadList::isAllThreadListItemChecked() const
{
    // Simple impl. but not fast:
    auto items = getItems<ThreadListItem>();
    for(ThreadListItem *item : items)
    {
        if(item->isCheckable() && !item->getCheckedState())
            return false;
    }
    return true;
}

void ThreadList::updateSelectAllItem()
{
    if(!m_DeleteMode)
        return;

    bool allChecked = isAllThreadListItemChecked();
    SelectAllListItem *selectAllItem = dynamic_cast<SelectAllListItem*>(getFirstItem());
    if(selectAllItem)
        selectAllItem->setCheckedState(allChecked, true);
}

void ThreadList::checkHandler(SelectAllListItem &item)
{
    bool checked = item.getCheckedState();
    checkAllItems(checked);
    if(m_pListener)
        m_pListener->onThreadListItemChecked();
}

void ThreadList::checkHandler(ThreadListItem &item)
{
    ThreadId threadId = item.getThreadId();
    MSG_LOG("Checked (id : state) = ", threadId, ":", item.getCheckedState());
    updateSelectAllItem();
    if(m_pListener)
        m_pListener->onThreadListItemChecked();
}

void ThreadList::updateList()
{
    MsgThreadListRef list = m_App.getMsgEngine().getStorage().getThreadList();
    if(!list)
        return;

    int length = list->getLength();
    for(int i = 0; i < length; ++i)
    {
        ThreadListItem *item = new ThreadListItem(list->at(i), m_App);
        ListView::appendItem(*item);
    }
}

void ThreadList::onMsgStorageChange(const MsgIdList &idList)
{
    ListView::clear(); // FIXME: temporary solution for demo
    updateList();
    if(m_pListener)
        m_pListener->onThreadListChanged();
}

void ThreadList::onContactChanged()
{
    ListView::clear(); // FIXME: temporary solution for demo
    updateList();
    if(m_pListener)
        m_pListener->onThreadListChanged();
}

void ThreadList::onListItemSelected(ListItem &listItem)
{
    ThreadListItem *it = dynamic_cast<ThreadListItem*>(&listItem);
    if(it && m_pListener)
        m_pListener->onListItemSelected(it->getThreadId());
}

void ThreadList::onListItemChecked(ListItem &listItem)
{
    if(ThreadListItem *it = dynamic_cast<ThreadListItem*>(&listItem))
        checkHandler(*it);
    else if(SelectAllListItem *it = dynamic_cast<SelectAllListItem*>(&listItem))
        checkHandler(*it);
}
