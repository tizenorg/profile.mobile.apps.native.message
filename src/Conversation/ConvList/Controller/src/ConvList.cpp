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

#include "ConvList.h"
#include "ConvListItem.h"
#include "Logger.h"

using namespace Msg;

ConvList::ConvList(Evas_Object *parent, MsgEngine &msgEngine, ThreadId threadId)
    : ConvListLayout(parent)
    , m_Mode(NormalMode)
    , m_MsgEngine(msgEngine)
    , m_ThreadId(threadId)
    , m_pSelectAll(nullptr)
    , m_pList(nullptr)
{
    create(parent);
}

ConvList::~ConvList()
{
    m_MsgEngine.getStorage().removeListener(*this);
}

void ConvList::setMode(ConvList::Mode mode)
{
    m_Mode = mode;
    bool isSelectMode = (m_Mode == ConvList::SelectMode);
    MSG_LOG("isSelectMode: ",isSelectMode);

    showSelectAllMode(isSelectMode);
    m_pList->setCheckMode(isSelectMode);
    if(isSelectMode)
    {
        auto items = m_pList->getItems();
        for(ListItem *it : items)
        {
            if(it->isCheckable())
                it->setCheckedState(false, false);
        }
    }
    m_pList->updateRealizedItems();
}

ConvList::Mode ConvList::getMode() const
{
    return m_Mode;
}

void ConvList::create(Evas_Object *parent)
{
    m_MsgEngine.getStorage().addListener(*this);
    Evas_Object *list = createList(parent);
    Evas_Object *selectAll = createSelectAll(parent);

    setSelectAll(selectAll);
    setBubbleList(list);
    fill();
    showSelectAllMode(m_Mode == SelectMode);
}

Evas_Object *ConvList::createSelectAll(Evas_Object *parent)
{
    m_pSelectAll = new ConvListSelectAllView(parent);
    m_pSelectAll->show();
    return *m_pSelectAll;
}

Evas_Object *ConvList::createList(Evas_Object *parent)
{
    m_pList = new ListView(parent);
    m_pList->setListener(this);
    m_pList->setMode(ELM_LIST_COMPRESS);
    m_pList->setHomogeneous(false);
    m_pList->show();

    return *m_pList;
}

void ConvList::fill()
{
    m_pList->clear();
    if(!m_ThreadId.isValid())
        return;

    MsgConversationListRef convList = m_MsgEngine.getStorage().getConversationList(m_ThreadId);
    int convListLen = convList->getLength();

    for(int i = 0; i < convListLen; ++i)
    {
        MsgConversationItem &item = convList->at(i);
        ConvListItem *listItem = new ConvListItem(item);
        m_pList->appendItem(*listItem);
    }
}

void ConvList::onListItemSelected(ListItem &listItem, void *funcData)
{
    ConvListItem &item = static_cast<ConvListItem&>(listItem);
    item.setSelected(false);
}

void ConvList::onListItemChecked(ListItem &listItem, void *funcData)
{
    ConvListItem &item = static_cast<ConvListItem&>(listItem);
}

void ConvList::onMsgStorageUpdate(const MsgIdList &msgIdList)
{
    // FIXME: simple impl for demo
    fill();
}

void ConvList::onMsgStorageInsert(const MsgIdList &msgIdList)
{
    // FIXME: simple impl for demo
    fill();
}

void ConvList::onMsgStorageDelete(const MsgIdList &msgIdList)
{
    // FIXME: simple impl for demo
    fill();
}

