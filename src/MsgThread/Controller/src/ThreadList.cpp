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
    inline SelectAllListItem *isSelectAll(ListItem *it)
    {
        return dynamic_cast<SelectAllListItem*>(it);
    }

    inline const SelectAllListItem *isSelectAll(const ListItem *it)
    {
        return dynamic_cast<const SelectAllListItem*>(it);
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
    ListView::setCmpFunc(cmpFunc);

    m_App.getMsgEngine().getStorage().addListener(*this);
    m_App.getContactManager().addListener(*this);
    m_App.getSysSettingsManager().addListener(*this);
    fillList();
}

ThreadList::~ThreadList()
{
    m_App.getMsgEngine().getStorage().removeListener(*this);
    m_App.getContactManager().removeListener(*this);
    m_App.getSysSettingsManager().removeListener(*this);
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
    auto items = getItems<ThreadListItem>();
    for(ThreadListItem *it : items)
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
            ++count;
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
            it->setCheckedState(check, false);
    }

    updateRealizedItems();
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

int ThreadList::cmpFunc(const ListItem &item1, const ListItem &item2)
{
    if(isSelectAll(&item1))
        return 0;

    if(isSelectAll(&item2))
        return 1;

    auto &threadItem1 = static_cast<const ThreadListItem&>(item1);
    auto &threadItem2 = static_cast<const ThreadListItem&>(item2);
    return threadItem2.getRawTime() - threadItem1.getRawTime();
}

void ThreadList::insertItem(const MsgThreadItem &item)
{
    ListView::sortedInsertItem(*new ThreadListItem(item, m_App));
}

void ThreadList::insertItem(ThreadId id)
{
    MsgThreadItemRef threadItem = m_App.getMsgEngine().getStorage().getThread(id);
    if(threadItem)
        insertItem(*threadItem);
}

ThreadListItem *ThreadList::getItem(ThreadId id) const
{
    auto items = ListView::getItems<ThreadListItem>();
    for(ThreadListItem *item : items)
    {
        if(item->getThreadId() == id)
            return item;
    }
    return nullptr;
}

void ThreadList::fillList()
{
    MsgThreadListRef msgThreadList = m_App.getMsgEngine().getStorage().getThreadList();
    if(!msgThreadList)
        return;

    int length = msgThreadList->getLength();

    for(int i = 0; i < length; ++i)
    {
        const MsgThreadItem &msgThreadItem = msgThreadList->at(i);
        ListView::appendItem(*new ThreadListItem(msgThreadItem, m_App));
    }
}

void ThreadList::deleteItems()
{
    MsgThreadListRef msgThreadList = m_App.getMsgEngine().getStorage().getThreadList();
    if(!msgThreadList)
        return;

    std::set<ThreadId> threadIdSet;
    int length = msgThreadList->getLength();
    for(int i = 0; i < length; ++i)
    {
        const MsgThreadItem &msgThreadItem = msgThreadList->at(i);
        threadIdSet.insert(msgThreadItem.getId());
    }

    auto items = ListView::getItems<ThreadListItem>();

    for(ThreadListItem *item : items)
    {
        if(threadIdSet.count(item->getThreadId()) == 0)
            ListView::deleteItem(*item);
    }

    updateSelectAllItem();
}

void ThreadList::updateItems(const MsgIdList &idList)
{
    auto threadIdSet = getThreadIdSet(idList);
    auto threadItems = getItems<ThreadListItem>();

    for(ThreadListItem *threadListItem: threadItems)
    {
        if(threadIdSet.count(threadListItem->getThreadId()))
            threadListItem->update();
    }

    updateRealizedItems();
}

void ThreadList::updateItems()
{
    auto items = getItems<ThreadListItem>();
    for(ThreadListItem *item: items)
    {
        item->update();
    }

    updateRealizedItems();
}

std::set<ThreadId> ThreadList::getThreadIdSet(const MsgIdList &idList)
{
    std::set<ThreadId> res;
    for(MsgId msgId : idList)
    {
        ThreadId threadId = m_App.getMsgEngine().getStorage().getThreadId(msgId);
        if(threadId.isValid())
            res.insert(threadId);
    }
    return res;
}

void ThreadList::onListItemSelected(ListItem &listItem)
{
    MSG_LOG("");
    ThreadListItem *it = dynamic_cast<ThreadListItem*>(&listItem);
    if(it && m_pListener)
        m_pListener->onListItemSelected(it->getThreadId());
}

void ThreadList::onListItemChecked(ListItem &listItem)
{
    MSG_LOG("");
    if(ThreadListItem *it = dynamic_cast<ThreadListItem*>(&listItem))
        checkHandler(*it);
    else if(SelectAllListItem *it = isSelectAll(&listItem))
        checkHandler(*it);
}

void ThreadList::onMsgStorageUpdate(const MsgIdList &msgIdList)
{
    MSG_LOG("");
    updateItems(msgIdList);
}

void ThreadList::onMsgStorageInsert(const MsgIdList &msgIdList)
{
    MSG_LOG("");

    bool inserted = false;
    bool updated = false;

    auto threadSet = getThreadIdSet(msgIdList);
    for(ThreadId id : threadSet)
    {
        ThreadListItem *item = getItem(id);
        if(item)
        {
            item->update();
            updated = true;
        }
        else
        {
            insertItem(id);
            inserted = true;
        }
    }

    if(inserted)
        updateSelectAllItem();
    if(updated)
        updateRealizedItems();

    if(m_pListener)
        m_pListener->onThreadListChanged();
}

void ThreadList::onMsgStorageDelete(const MsgIdList &msgIdList)
{
    MSG_LOG("");
    deleteItems();
    updateItems(); // TODO: Inefficiently. How to get list of threads by dead msgIdList ?
    if(m_pListener)
        m_pListener->onThreadListChanged();
}

void ThreadList::onContactChanged()
{
    MSG_LOG("");
    updateItems();
}

void ThreadList::onTimeFormatChanged()
{
    MSG_LOG("");
    auto items = ListView::getItems<ThreadListItem>();
    for(ThreadListItem *item : items)
    {
        item->updateTime();
    }

    ListView::updateRealizedItems();
}
