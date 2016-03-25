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

#include "MsgOnSimCard.h"
#include "Config.h"
#include "MsgEngine.h"
#include "Logger.h"
#include "PathUtils.h"
#include "Logger.h"
#include "App.h"
#include "Popup.h"
#include "ListItem.h"
#include <notification_status.h>

#include <Elementary.h>
#include <sstream>

using namespace Msg;

MsgOnSimCard::MsgOnSimCard(NaviFrameController &parent)
    : FrameController(parent)
    , m_pList(nullptr)
    , m_SimMode(NormalMode)
    , m_CheckCount(0)
{
    create();
    getApp().getMsgEngine().getStorage().addListener(*this);
}

MsgOnSimCard::~MsgOnSimCard()
{
    getApp().getMsgEngine().getStorage().removeListener(*this);
}

void MsgOnSimCard::create()
{
    m_pList = new ListView(getParent());
    m_pList->setListener(this);
    m_pList->expand();
    m_pList->setMultiSelection(false);
    m_pList->show();
    m_pList->setMode(ELM_LIST_COMPRESS);
    fillList();
}

void MsgOnSimCard::fillList()
{
    MessageSMSListRef list = getMsgEngine().getStorage().getSimMsgList();
    int length = list->getLength();
    for(int i = 0; i < length; ++i)
    {
        SimListViewItem *item = new SimListViewItem(list->at(i));
        m_pList->appendItem(*item);
    }
}

void MsgOnSimCard::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    getNaviBar().setColor(NaviBar::NaviWhiteColorId);
    setTitleWithButtons(false);
    setHwButtonListener(*m_pList, this);
    setContent(*m_pList);
}

void MsgOnSimCard::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    if(buttonId == NaviOkButtonId)
    {
        if (m_SimMode == DeleteMode)
        {
            deleteSelectedItems();
        }
        else if (m_SimMode == CopyToDeviceMode)
        {
            copySelectedItems();
        }
    }
    else if(buttonId == NaviPrevButtonId)
        getParent().pop();
    setMode(NormalMode);
}

void MsgOnSimCard::onListItemChecked(ListItem &listItem)
{
    if(SelectAllListItem *it = dynamic_cast<SelectAllListItem*>(&listItem))
    {
        checkHandler(*it);
    }
    else
    {
        MSG_LOG_ERROR("dynamic_cast was FAILED!");
    }

    int check = 0;
    int total = 0;
    calcChecked(check, total);
    m_CheckCount = check;
    getNaviBar().disabledButton(NaviOkButtonId, m_CheckCount == 0);
    bool all = (check == total) ? true : false;
    m_pList->getFirstItem()->setCheckedState(all, true);

    setTitleTranslatable();
}

void MsgOnSimCard::checkHandler(SelectAllListItem &item)
{
    bool checked = item.getCheckedState();
    m_pList->checkAllItems(checked);
    getNaviBar().disabledButton(NaviOkButtonId, !checked);
}

void MsgOnSimCard::onHwMoreButtonClicked()
{
    showCopyDeletePopup();
}

void MsgOnSimCard::onHwBackButtonClicked()
{
    if(m_SimMode == CopyToDeviceMode || m_SimMode == DeleteMode)
        setNormalMode();
    else
        getParent().pop();
}

void MsgOnSimCard::showCopyDeletePopup()
{
    ContextPopup &popup = getApp().getPopupManager().getCtxPopup();
    popup.appendItem(msg("IDS_MSG_OPT_COPY_TO_DEVICE_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(MsgOnSimCard, onCopyToDeviceItemPressed), this);
    popup.appendItem(msg("IDS_MSG_OPT_DELETE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(MsgOnSimCard, onDeleteItemPressed), this);
    popup.align(getApp().getWindow());
    popup.show();
}

void MsgOnSimCard::onCopyToDeviceItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();
    setMode(CopyToDeviceMode);
}

void MsgOnSimCard::onDeleteItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();
    setMode(DeleteMode);
}

void MsgOnSimCard::onMsgStorageDelete(const MsgIdList &msgIdList)
{
    auto simListItems = m_pList->getItems<SimListViewItem>();
    for(auto &itemId: msgIdList)
    {
        for(auto *simItem : simListItems)
        {
            if(simItem->getMsgId() == itemId)
            {
                simItem->destroy();
                break;
            }
        }
    }
}

void MsgOnSimCard::setMode(SimMode mode)
{
    if(m_SimMode == mode)
        return;

    switch(mode)
    {
        case CopyToDeviceMode:
            setCopyToDeviceMode(true);
            break;

        case DeleteMode:
            setDeleteMode(true);
            break;

        case NormalMode:
            setNormalMode();
            break;

        default:
            MSG_ASSERT(false, "Not supported mode");
            break;
    }
}

void MsgOnSimCard::setNormalMode()
{
    // Reset previus mode:
    switch(m_SimMode)
    {
        case CopyToDeviceMode:
            setCopyToDeviceMode(false);
            break;

        case DeleteMode:
            setDeleteMode(false);
            break;

        default:
            break;
    }

    m_SimMode = NormalMode;
    m_CheckCount = 0;
    setTitleTranslatable();
}

void MsgOnSimCard::setCopyToDeviceMode(bool value)
{
    if(value)
        m_SimMode = CopyToDeviceMode;

    setTitleWithButtons(value);
}

void MsgOnSimCard::setDeleteMode(bool value)
{
    if(value)
        m_SimMode = DeleteMode;

    setTitleWithButtons(value);
}

void MsgOnSimCard::setTitleTranslatable()
{
    if(m_SimMode == NormalMode)
        getNaviBar().setTitle(msg("IDS_MSG_HEADER_MESSAGES_ON_SIM_CARD_ABB"));
    else if(m_SimMode == DeleteMode || m_SimMode == CopyToDeviceMode)
        getNaviBar().setTitle(msgArgs("IDS_MSG_HEADER_PD_SELECTED_ABB3", m_CheckCount));
}

void MsgOnSimCard::setTitleWithButtons(bool value)
{
    setTitleTranslatable();

    getNaviBar().clear();
    getNaviBar().showButton(NaviOkButtonId, value);
    if(value)
        getNaviBar().disabledButton(NaviOkButtonId, true);
    getNaviBar().showButton(NaviCancelButtonId, value);
    getNaviBar().showButton(NaviPrevButtonId, !value);

    m_pList->setCheckMode(value);
    m_pList->checkAllItems(false);
    showSelectAllItem(value);
}

void MsgOnSimCard::deleteSelectedItems()
{
    auto items = m_pList->getItems<SimListViewItem>();

    MsgIdList messages;
    for(auto *item : items)
        if(item->getCheckedState())
            messages.push_back(item->getMsgId());

    getApp().getMsgEngine().getStorage().deleteMessages(messages);
}

void MsgOnSimCard::copySelectedItems()
{
    auto items = m_pList->getItems<SimListViewItem>();
    for(auto *item : items)
    {
        if(item->getCheckedState())
        {
            MessageRef msg = getApp().getMsgEngine().getStorage().getMessage(item->getMsgId());
            if(msg)
            {
                msg->setMessageStorageType(Message::MS_Phone);
                getApp().getMsgEngine().getStorage().saveMessage(*msg, false);
            }
        }
    }
    notification_status_message_post(msg("IDS_MSG_TPOP_SELECTED_MESSAGES_COPIED_TO_MOBILE_DEVICE").cStr());
}

void MsgOnSimCard::showSelectAllItem(bool show, bool resetCheck)
{
    SelectAllListItem *item = dynamic_cast<SelectAllListItem*>(m_pList->getFirstItem());

    if(show)
    {
        if(!item)
        {
            item = new SelectAllListItem;
            m_pList->prependItem(*item);
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

void MsgOnSimCard::calcChecked(int &check, int &total) const
{
    check = 0;
    total = 0;
    auto items = m_pList->getItems<SimListViewItem>();
    for(ListItem *item: items)
    {
        if (item->getCheckedState())
            ++check;
        ++total;
    }
}
