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

#include "MsgThread.h"
#include "Settings.h"
#include "Conversation.h"
#include "Logger.h"
#include "ThreadListView.h"
#include "ThreadListItem.h"
#include "ResourceUtils.h"
#include "MsgEngine.h"
#include "App.h"
#include "ThreadListItem.h"
#include "ThreadListItemType.h"
#include "SelectAllListItem.h"
#include "Popup.h"
#include "ContactManager.h"

#include <Elementary.h>
#include <sstream>

using namespace Msg;

enum CtxPopupItemId
{
    ComposeId,
    SearchId,
    DeleteId,
    SettingsId
};

MsgThread::MsgThread(NaviFrameController &parent)
    : FrameController(parent)
    , m_pLayout(nullptr)
    , m_pThreadListView(nullptr)
    , m_pSearchPanel(nullptr)
    , m_Mode(InitMode)
{
    m_pLayout = new MsgThreadLayout(getParent());
    m_pLayout->show();

    m_pThreadListView = new ThreadListView(*m_pLayout);
    m_pThreadListView->setListener(this);
    m_pThreadListView->setMultiSelection(false);
    m_pThreadListView->show();

    m_pLayout->setList(*m_pThreadListView);

    fillThreadList();

    getMsgEngine().getStorage().addListener(*this);

    setMode(NormalMode);
}

MsgThread::~MsgThread()
{
    getMsgEngine().getStorage().removeListener(*this);
}

void MsgThread::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    getNaviBar().setTitle(msgt("IDS_MSG_HEADER_MESSAGES"));
    getNaviBar().setColor(NaviBar::NaviBlueColorId);
    getNaviBar().setSearch(createSearchPanel(getNaviBar()));
    setHwButtonListener(*m_pLayout, this);
    setContent(*m_pLayout);
}

void MsgThread::showMainCtxPopup()
{
    resetCtxPopup();
    getCtxPopup().appendItem(CtxPopupItemId::ComposeId, "Compose");
    getCtxPopup().appendItem(CtxPopupItemId::SearchId, "Search");
    getCtxPopup().appendItem(CtxPopupItemId::DeleteId, "Delete");
    getCtxPopup().appendItem(CtxPopupItemId::SettingsId, "Settings");
    getCtxPopup().align(getApp().getWindow());
    getCtxPopup().show();
}

void MsgThread::fillThreadList()
{
    MsgThreadListRef list = getMsgEngine().getStorage().getThreadList();

    int length = list->getLength();
    for(int i = 0; i < length; ++i)
    {
        ThreadListItem *item = new ThreadListItem(list->at(i), getApp());
        m_pThreadListView->appendItem(item->getViewItem());
    }
}

void MsgThread::composeNewMessage()
{
    Conversation *frame = new Conversation(getParent());
    getParent().push(*frame);
}

void MsgThread::navigateToSettings()
{
    Settings *frame = new Settings(getParent());
    getParent().push(*frame);
}

void MsgThread::navigateToConversation(ThreadId threadId)
{
    Conversation *frame = new Conversation(getParent(), threadId);
    getParent().push(*frame);
}

void MsgThread::setMode(Mode mode)
{
    if(m_Mode == mode)
        return;

    setNormalMode();

    switch(mode)
    {
        case NormalMode:
            break;

        case DeleteMode:
            setDeleteMode(true);
            break;

        case SearchMode:
            setSearchMode(true);
            break;

        case InitMode:
        default:
            MSG_ASSERT(false, "Not supported mode");
            break;
    }
}

void MsgThread::setNormalMode()
{
    // Reset previus mode:
    switch(m_Mode)
    {
        case DeleteMode:
            setDeleteMode(false);
            break;

        case SearchMode:
            setSearchMode(false);
            break;

        default:
            break;
    }

    m_Mode = NormalMode;
}

void MsgThread::setDeleteMode(bool value)
{
    if(value)
        m_Mode = DeleteMode;

    getNaviBar().showButton(NaviOkButtonId, value);
    getNaviBar().showButton(NaviCancelButtonId, value);

    m_pThreadListView->setCheckMode(value);
    m_pThreadListView->checkAllItems(false);
    m_pThreadListView->showSelectAllItem(value);
}

void MsgThread::checkHandler(SelectAllListItem &item)
{
    bool checked = item.getCheckedState();
    m_pThreadListView->checkAllItems(checked);
}

void MsgThread::checkHandler(ThreadListItem &item)
{
    ThreadId threadId = item.getThreadId();
    MSG_LOG("Checked (id : state) = ", threadId, ":", item.getCheckedState());
}

void MsgThread::selectHandler(ThreadListItem &item)
{
    ThreadId threadId = item.getThreadId();
    MSG_LOG("Selected MsgThreadItem id = ", threadId);
    navigateToConversation(threadId);
}

void MsgThread::onHwBackButtonClicked()
{
    if(m_Mode == SearchMode || m_Mode == DeleteMode)
    {
        setMode(NormalMode);
    }
    else
    {
        getParent().pop();
    }
}

void MsgThread::onHwMoreButtonClicked()
{
    if(m_Mode == NormalMode)
        showMainCtxPopup();
}

void MsgThread::onListItemSelected(ListItem &listItem, void *funcData)
{
    listItem.setSelected(false);

    if(ThreadListItem *it = dynamic_cast<ThreadListItem*>(&listItem))
    {
        selectHandler(*it);
    }
}

void MsgThread::onListItemChecked(ListItem &listItem, void *funcData)
{
    if(ThreadListItem *it = dynamic_cast<ThreadListItem*>(&listItem))
    {
        checkHandler(*it);
    }
    else if(SelectAllListItem *it = dynamic_cast<SelectAllListItem*>(&listItem))
    {
        checkHandler(*it);
    }
}

void MsgThread::onPopupButtonClicked(Popup &popup, int buttonId)
{
    MSG_LOG("Popup button id: ", buttonId);
    resetPopup();
}

void MsgThread::onMsgStorageChange()
{
    m_pThreadListView->clear(); // FIXME: temporary solution for demo
    fillThreadList();
}

void MsgThread::onContextPopupItemPressed(ContextPopup &ctxPopup, ContextPopupItem &item)
{
    MSG_LOG("ContextPopup item id: ", item.getId());
    ctxPopup.dismiss();

    switch(item.getId())
    {
        case CtxPopupItemId::ComposeId:
            composeNewMessage();
            break;

        case CtxPopupItemId::SearchId:
            setMode(SearchMode);
            break;

        case CtxPopupItemId::DeleteId:
            setMode(DeleteMode);
            break;

        case CtxPopupItemId::SettingsId:
            navigateToSettings();
            break;

        default:
            MSG_ASSERT(false, "Unknown item id");
            break;
    }
}

void MsgThread::deleteSelectedItems()
{
    auto collection = m_pThreadListView->getItems<ThreadListItem>();
    for(ThreadListItem *it : collection)
    {
        if(it->getCheckedState())
        {
            getMsgEngine().getStorage().deleteThread(it->getThreadId());
        }
    }
}

void MsgThread::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    MSG_LOG("NaviButton id:", buttonId);
    if(buttonId == NaviOkButtonId)
    {
        deleteSelectedItems();
    }
    setMode(NormalMode);
}
