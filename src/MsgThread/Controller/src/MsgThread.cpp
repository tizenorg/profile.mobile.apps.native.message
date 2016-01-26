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
#include "App.h"
#include "Popup.h"

using namespace Msg;

MsgThread::MsgThread(NaviFrameController &parent)
    : FrameController(parent)
    , m_pLayout(nullptr)
    , m_pNoContent(nullptr)
    , m_pThreadList(nullptr)
    , m_pSearchList(nullptr)
    , m_pSearchPanel(nullptr)
    , m_Mode(InitMode)
    , m_pFloatingBtn(nullptr)
{
    m_pLayout = new MsgThreadLayout(getParent());
    m_pLayout->show();

    m_pFloatingBtn = new FloatingButton(*m_pLayout);
    m_pFloatingBtn->show();
    m_pFloatingBtn->setListener(this);
    m_pLayout->setFloatingButton(*m_pFloatingBtn);

    m_pNoContent = new NoContentLayout(*m_pLayout);
    m_pNoContent->show();

    m_pThreadList = new ThreadList(*m_pLayout, getApp());
    m_pThreadList->setListener(this);
    m_pThreadList->show();

    m_pSearchList = new ThreadSearchList(*m_pLayout, getApp());
    m_pSearchList->setListener(this);
    m_pSearchList->show();

    m_pLayout->setNoContent(*m_pNoContent);
    m_pLayout->setList(*m_pThreadList);
    m_pLayout->setSearchList(*m_pSearchList);

    setMode(NormalMode);
}

MsgThread::~MsgThread()
{
}

Evas_Object *MsgThread::createSearchPanel(Evas_Object *parent)
{
    m_pSearchPanel = new MsgThreadSearchPanel(*m_pLayout);
    m_pSearchPanel->setListener(this);
    m_pSearchPanel->setGuideText("Search");
    m_pSearchPanel->show();
    return *m_pSearchPanel;
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
    auto &popupMngr = getApp().getPopupManager();
    // TODO: localization:
    popupMngr.getCtxPopup().appendItem("Search", nullptr, CTXPOPUP_ITEM_PRESSED_CB(MsgThread, onSearchItemPressed), this);
    popupMngr.getCtxPopup().appendItem("Delete", nullptr, CTXPOPUP_ITEM_PRESSED_CB(MsgThread, onDeleteItemPressed), this);
    popupMngr.getCtxPopup().appendItem("Settings", nullptr, CTXPOPUP_ITEM_PRESSED_CB(MsgThread, onSettingsItemPressed), this);
    popupMngr.getCtxPopup().align(getApp().getWindow());
    popupMngr.getCtxPopup().show();
}

void MsgThread::composeNewMessage()
{
    Conversation *frame = new Conversation(getParent());
    frame->setThreadId(ThreadId());
    getParent().push(*frame);
}

void MsgThread::navigateToSettings()
{
    Settings *frame = new Settings(getParent());
    getParent().push(*frame);
}

void MsgThread::navigateToConversation(ThreadId threadId, MsgId msgId)
{
    Conversation *frame = new Conversation(getParent());
    frame->setThreadId(threadId);
    if(msgId.isValid())
        frame->navigateTo(msgId);
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

        case SearchMode:
            setSearchMode(true);
            break;

        case DeleteMode:
            setDeleteMode(true);
            break;

        case InitMode:
        default:
            MSG_ASSERT(false, "Not supported mode");
            break;
    }
}

void MsgThread::setNormalMode()
{
    MSG_LOG("");
    getNaviBar().clear();
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

    m_pNoContent->setText(msgt("IDS_MSG_NPBODY_NO_MESSAGES"));
    m_Mode = NormalMode;
    update();
}

void MsgThread::setDeleteMode(bool value)
{
    MSG_LOG("");

    if(value)
        m_Mode = DeleteMode;

    getNaviBar().showButton(NaviOkButtonId, value);
    getNaviBar().showButton(NaviCancelButtonId, value);
    m_pThreadList->setDeleteMode(value);
}

void MsgThread::setSearchMode(bool value)
{
    MSG_LOG("");

    getNaviBar().showButton(NaviPrevButtonId, value);
    if(value)
    {
        m_Mode = SearchMode;
        m_pSearchPanel->clearEntry();
        m_pSearchList->clear();
        getNaviBar().showSearch();
        m_pNoContent->setText(msgt("IDS_MSG_NPBODY_NO_RESULTS_FOUND_ABB"));
    }
    else
    {
        m_pSearchList->cancelSearch();
        getNaviBar().hideSearch();
    }

    update();
    m_pSearchPanel->setEntryFocus(value);
}

void MsgThread::update()
{
    bool searchEnabled = m_Mode == SearchMode;

    m_pLayout->showFloatingButton(!searchEnabled);
    if(searchEnabled)
    {
        bool showSearch = !m_pSearchList->isEmpty();
        bool showThread = !showSearch && !m_pThreadList->isEmpty() && m_pSearchList->getSearchWord().empty();
        bool showNoContent = !showThread && !showSearch;

        m_pLayout->showSearchList(showSearch);
        m_pLayout->showThreadList(showThread);
        m_pLayout->showNoContent(showNoContent);

    }
    else
    {
        bool showThread = !m_pThreadList->isEmpty();
        m_pLayout->showThreadList(showThread);
        m_pLayout->showNoContent(!showThread);
        m_pLayout->showSearchList(false);
    }
}

void MsgThread::search(const std::string &searchWord)
{
    if(m_Mode == SearchMode)
        m_pSearchList->requestSearch(searchWord);
}

void MsgThread::onHwBackButtonClicked()
{
    MSG_LOG("");
    if(m_Mode == SearchMode || m_Mode == DeleteMode)
        setMode(NormalMode);
    else
        getParent().pop();
}

void MsgThread::onHwMoreButtonClicked()
{
    MSG_LOG("");
    if(m_Mode == NormalMode)
        showMainCtxPopup();
}

void MsgThread::onSettingsItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    navigateToSettings();
}

void MsgThread::onDeleteItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    setMode(DeleteMode);
}

void MsgThread::onSearchItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    setMode(SearchMode);
}

void MsgThread::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    MSG_LOG("NaviButton id:", buttonId);
    if(buttonId == NaviOkButtonId)
        m_pThreadList->deleteSelectedItems();
    setMode(NormalMode);
}

void MsgThread::onListItemSelected(ThreadId id)
{
    navigateToConversation(id);
}

void MsgThread::onThreadListChanged()
{
    MSG_LOG("");
    update();
}

void MsgThread::onSearchListChanged()
{
    MSG_LOG("");
    update();
}

void MsgThread::onSearchListItemSelected(ThreadId id)
{
    MSG_LOG("");
    navigateToConversation(id);
}

void MsgThread::onSearchListItemSelected(MsgId id)
{
    MSG_LOG("");
    MessageRef msg = getMsgEngine().getStorage().getMessage(id);
    if(msg)
        navigateToConversation(msg->getThreadId(), id);
}

void MsgThread::onFloatingButtonPressed()
{
    MSG_LOG("");
    composeNewMessage();
}

void MsgThread::onSearchButtonClicked(MsgThreadSearchPanel &obj)
{
    MSG_LOG("");
}

void MsgThread::onEntryChanged(MsgThreadSearchPanel &obj)
{
    MSG_LOG("");
    search(obj.getEntryText());
}
