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
#include "Logger.h"
#include "CallbackAssist.h"
#include "TimeUtils.h"
#include "DateLineItem.h"

using namespace Msg;

namespace
{
    const int minMessagesBulk = 100;
    const int additionalMessagesBulk = 50;
}

ConvList::ConvList(Evas_Object *parent, App &app, WorkingDirRef workingDir)
    : ConvListLayout(parent)
    , m_Mode(NormalMode)
    , m_MsgEngine(app.getMsgEngine())
    , m_pSelectAll(nullptr)
    , m_pList(nullptr)
    , m_ConvListItemMap()
    , m_DateLineItemSet()
    , m_pListner(nullptr)
    , m_App(app)
    , m_WorkingDir(workingDir)
    , m_OwnerThumbId(m_App.getThumbnailMaker().getThumbId(ThumbnailMaker::OwnerThumb))
    , m_RecipThumbId(m_App.getThumbnailMaker().getThumbId(ThumbnailMaker::SingleThumb))
    , m_SearchWord()
{
    create(parent);
}

ConvList::~ConvList()
{
    m_MsgEngine.getStorage().removeListener(*this);
    m_App.getContactManager().removeListener(*this);
    m_App.getSysSettingsManager().removeListener(*this);
}

void ConvList::setListener(IConvListListener *l)
{
    m_pListner = l;
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
        selectListItems(false);
        m_pSelectAll->setCheckState(false);
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
    showSelectAllMode(m_Mode == SelectMode);

    m_App.getContactManager().addListener(*this);
    m_App.getSysSettingsManager().addListener(*this);
}

Evas_Object *ConvList::createSelectAll(Evas_Object *parent)
{
    m_pSelectAll = new ConvSelectAll(parent);
    m_pSelectAll->addCheckCallback(SMART_CALLBACK(ConvList, onSelectAllChanged), this);
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
    clear();
    if(!m_ThreadId.isValid())
        return;

    MsgConversationListRef convList = m_MsgEngine.getStorage().getConversationList(m_ThreadId);
    int convListLen = convList->getLength();
    int reserveSize = convListLen <= minMessagesBulk/2 ? minMessagesBulk : convListLen + additionalMessagesBulk;
    m_ConvListItemMap.reserve(reserveSize);
    m_DateLineItemSet.reserve(reserveSize);

    for(int i = 0; i < convListLen; ++i)
    {
        MsgConversationItem &item = convList->at(i);
        const ThumbnailMaker::ThumbId &thumbId = item.getDirection() == Message::MD_Received ? m_RecipThumbId : m_OwnerThumbId;
        appendItem(new ConvListItem(item, m_App, m_WorkingDir, m_SearchWord, thumbId));
    }
}

void ConvList::setThreadId(ThreadId id, const std::string &searchWord)
{
    if(m_ThreadId != id || m_SearchWord != searchWord)
    {
        m_ThreadId = id;
        m_SearchWord = searchWord;
        updateRecipThumbId();
        fill();
    }
}

void ConvList::updateRecipThumbId()
{
    const MsgAddressListRef addressList = m_App.getMsgEngine().getStorage().getAddressList(m_ThreadId);
    if(addressList)
    {
        int countContact = addressList->getLength();
        if(countContact > 1)
            m_RecipThumbId = m_App.getThumbnailMaker().getThumbId(ThumbnailMaker::GroupThumb);
        else if(countContact == 1)
            m_RecipThumbId = m_App.getThumbnailMaker().getThumbId(addressList->at(0));
        else
        {
            m_RecipThumbId = ThumbnailMaker::SingleThumb;
            MSG_LOG_WARN("Msg address list is empty");
        }
    }
}

void ConvList::updateOwnerThumbId()
{
    m_OwnerThumbId = m_App.getThumbnailMaker().getThumbId(ThumbnailMaker::OwnerThumb);
}

void ConvList::navigateTo(MsgId msgId)
{
    ConvListItem *item = getItem(msgId);
    if(item)
        m_pList->showItem(*item, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE);
}

void ConvList::navigateToLastMsg()
{
    ListItem *item = m_pList->getLastItem();
    if(item)
        m_pList->showItem(*item, ELM_GENLIST_ITEM_SCROLLTO_TOP);
}

ConvListItem *ConvList::getItem(MsgId msgId) const
{
    auto it = m_ConvListItemMap.find(msgId);
    return it != m_ConvListItemMap.end() ? it->second : nullptr;
}

void ConvList::appendItem(ConvListItem *item)
{
    dateLineAddIfNec(item);
    m_ConvListItemMap[item->getMsgId()] = item;
    item->setListener(this);
    m_pList->appendItem(*item);
}

void ConvList::deleteItem(ConvListItem *item)
{
    dateLineDelIfNec(item);
    m_ConvListItemMap.erase(item->getMsgId());
    m_pList->deleteItem(*item);
}

void ConvList::demoteItem(ConvListItem *item)
{
    dateLineDelIfNec(item);
    dateLineAddIfNec(item);
    m_pList->demoteItem(*item);
}

void ConvList::dateLineDelIfNec(ConvListItem *item)
{
    bool needDelDateLine = false;
    DateLineItem *prev = dynamic_cast<DateLineItem*>(m_pList->getPrevItem(*item));
    if(prev)
    {
        ListItem *nextItem = m_pList->getNextItem(*item);
        needDelDateLine = nextItem ? dynamic_cast<DateLineItem*>(nextItem) != nullptr : true;
    }

    if(needDelDateLine)
    {
        m_DateLineItemSet.erase(prev->getDateLine());
        m_pList->deleteItem(*prev);
    }
}

void ConvList::dateLineAddIfNec(ConvListItem *item)
{
    std::string dateStr = TimeUtils::makeBubbleDateLineString(item->getRawTime());
    auto it = m_DateLineItemSet.find(dateStr);
    if (it == m_DateLineItemSet.end())
    {
        DateLineItem *dateLine = new DateLineItem(item->getRawTime(), dateStr);
        m_DateLineItemSet.insert(dateStr);
        m_pList->appendItem(*dateLine);
    }
}

void ConvList::clear()
{
    m_pList->clear();
    m_ConvListItemMap.clear();
    m_DateLineItemSet.clear();
}

void ConvList::deleteSelectedItems()
{
    auto items = m_pList->getItems<ConvListItem>();
    MsgIdList messages;
    for(ConvListItem *item : items)
    {
        if(item->getCheckedState())
            messages.push_back(item->getMsgId());
    }
    m_MsgEngine.getStorage().deleteMessages(messages);
}

int ConvList::getMessageCount() const
{
    auto items = m_pList->getItems<ConvListItem>();
    return (int)items.size();
}

int ConvList::getMessageCheckedCount() const
{
    auto items = m_pList->getItems<ConvListItem>();
    int count = 0;
    for(ConvListItem *item : items)
    {
        if(item->getCheckedState())
            count++;
    }
    return count;
}

bool ConvList::isAllListItemSelected() const
{
    // Simple but not fast impl:
    auto items = m_pList->getItems<ConvListItem>();
    for(ConvListItem *item : items)
    {
        if(!item->getCheckedState())
            return false;
    }
    return true;
}

void ConvList::selectListItems(bool state)
{
    auto items = m_pList->getItems<ConvListItem>();
    for(ConvListItem *item : items)
    {
        item->setCheckedState(state, false);
    }
    m_pList->updateRealizedItems();
    if(m_pListner)
        m_pListner->onConvListItemChecked();
}

void ConvList::onListItemLongPressed(ListItem &listItem)
{
    ConvListItem &item = static_cast<ConvListItem&>(listItem);
    item.showPopup();
}

void ConvList::onSelectAllChanged(Evas_Object *obj, void *eventInfo)
{
    selectListItems(m_pSelectAll->getCheckState());
}

void ConvList::onListItemChecked(ListItem &listItem)
{
    bool allSelected = isAllListItemSelected();
    m_pSelectAll->setCheckState(allSelected);

    if(m_pListner)
        m_pListner->onConvListItemChecked();
}

void ConvList::onMsgStorageUpdate(const MsgIdList &msgIdList)
{
    for(auto &itemId: msgIdList)
    {
        ConvListItem *updated = getItem(itemId);
        if(updated)
        {
            updated->updateStatus();
            demoteItem(updated);
        }
    }
}

void ConvList::onMsgStorageInsert(const MsgIdList &msgIdList)
{
    bool inserted = false;
    for(MsgId msgId: msgIdList)
    {
        if(getItem(msgId))
            continue;

        MessageRef msg = m_MsgEngine.getStorage().getMessage(msgId);
        if(msg && msg->getThreadId() == m_ThreadId && msg->getMessageStorageType() != Message::MS_Sim)
        {
            MsgConversationItemRef item = m_MsgEngine.getStorage().getConversationItem(msgId);
            const ThumbnailMaker::ThumbId &thumbId = item->getDirection() == Message::MD_Received ? m_RecipThumbId : m_OwnerThumbId;
            appendItem(new ConvListItem(*item, m_App, m_WorkingDir, m_SearchWord, thumbId));
            inserted = true;
        }
    }
    if(inserted)
        navigateToLastMsg();
}

void ConvList::onMsgStorageDelete(const MsgIdList &msgIdList)
{
    for(auto &itemId: msgIdList)
    {
        ConvListItem *deleted = getItem(itemId);
        if(deleted)
            deleteItem(deleted);
    }

    if(m_pListner && m_pList->isEmpty())
        m_pListner->onAllItemsDeleted(*this);
}

void ConvList::onForwardMsg(ConvListItem &item)
{
    if(m_pListner)
        m_pListner->onForwardMsg(item.getMsgId());
}

void ConvList::onSlideShow(ConvListItem &item)
{
    if(m_pListner)
        m_pListner->onSlideShow(item.getMsgId());
}

void ConvList::onEditDraftMsg(ConvListItem &item)
{
    if(m_pListner)
        m_pListner->onEditDraftMsg(item.getMsgId());
}

void ConvList::onContactChanged()
{
    MSG_LOG("");
    updateRecipThumbId();
    updateOwnerThumbId();
    m_pList->updateRealizedItems();
}

void ConvList::onTimeFormatChanged()
{
    MSG_LOG("");
    auto items = m_pList->getItems<ConvListItem>();
    for(ConvListItem *item : items)
    {
        item->updateTime();
    }
    m_pList->updateRealizedItems();
}

void ConvList::onLanguageChanged()
{
    MSG_LOG("");

    // Update ConvListItem:
    auto convListItems = m_pList->getItems<ConvListItem>();
    for(ConvListItem *item : convListItems)
    {
        item->updateTime();
    }

    // Update DateLineItem:
    m_DateLineItemSet.clear();
    auto DateLineItems = m_pList->getItems<DateLineItem>();
    for(DateLineItem *item : DateLineItems)
    {
        item->update();
        m_DateLineItemSet.insert(item->getDateLine());
    }

    m_pList->updateRealizedItems();
}
