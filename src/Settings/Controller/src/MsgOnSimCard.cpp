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
#include "ResourceUtils.h"
#include "Logger.h"
#include "App.h"
#include "Popup.h"

#include <Elementary.h>
#include <sstream>

using namespace Msg;

enum popupItemId
{
    CopyToDeviceId,
    DeleteId
};


MsgOnSimCard::MsgOnSimCard(NaviFrameController &parent)
    : FrameController(parent)
    , m_pList(nullptr)
    , m_SimMode(NormalMode)
    , m_CheckCount(0)
{
    create();
}

MsgOnSimCard::~MsgOnSimCard()
{
}

void MsgOnSimCard::create()
{
    m_pList = new ListView(getParent().getEvasObject());
    m_pList->setListener(this);
    m_pList->expand();
    m_pList->setMultiSelection(false);
    m_pList->show();
    m_pList->setMode(ELM_LIST_COMPRESS);

    SimListViewItem *item = new SimListViewItem;
    m_pList->appendItem(*item);
}

void MsgOnSimCard::onViewItemCreated()
{
    setTitleTranslatable();
    setHwButtonListener(getContent(), this);

 /*   auto list = getMsgEngine().getStorage().getSimMsgList();
    for(auto msg : list)
    {
        MSG_LOG(msg->getNumber());
        MSG_LOG(msg->getText());
        MSG_LOG(msg->getTime());
    }*/
}

void MsgOnSimCard::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    if(buttonId == NaviOkButtonId)
    {
        if (m_SimMode == DeleteMode)
        {
            //TODO: deleteSelectedItems();
        }
        else if (m_SimMode == CopyToDeviceMode)
        {
            //TODO: copySelectedItems();
        }
    }
    setMode(NormalMode);
}

Evas_Object *MsgOnSimCard::getContent()
{
    return *m_pList;
}

void MsgOnSimCard::onListItemChecked(ListItem &listItem, void *funcData)
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
    bool all = (check == total) ? true : false;
    m_pList->getFirstItem()->setCheckedState(all, true);

    setTitleTranslatable();
}

void MsgOnSimCard::checkHandler(SelectAllListItem &item)
{
    bool checked = item.getCheckedState();
    m_pList->checkAllItems(checked);
}

void MsgOnSimCard::onHwMoreButtonClicked()
{
    showCopyDeletePopup();
}

void MsgOnSimCard::onHwBackButtonClicked()
{
    if(m_SimMode == CopyToDeviceMode || m_SimMode == DeleteMode)
    {
        setNormalMode();
    }
    else
    {
        getParent().pop();
    }
}

void MsgOnSimCard::showCopyDeletePopup()
{
    resetCtxPopup();
    getCtxPopup().appendItem(popupItemId::CopyToDeviceId, msg("IDS_MSG_OPT_COPY_TO_DEVICE_ABB"));
    getCtxPopup().appendItem(popupItemId::DeleteId, msg("IDS_MSG_OPT_DELETE"));
    getCtxPopup().align(getApp().getWindow());
    getCtxPopup().show();
}

void MsgOnSimCard::onContextPopupItemPressed(ContextPopup &ctxPopup, ContextPopupItem &item)
{
    ctxPopup.dismiss();

    switch(item.getId())
    {
        case popupItemId::CopyToDeviceId:
            setMode(CopyToDeviceMode);
            break;

        case popupItemId::DeleteId:
            setMode(DeleteMode);
            break;

        default:
            MSG_ASSERT(false, "Unknown item id");
            break;
    }
}

void MsgOnSimCard::setMode(SimMode mode)
{
    if(m_SimMode == mode)
    {
        return;
    }

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
    {
        m_SimMode = DeleteMode;
    }

    setTitleWithButtons(value);
}

void MsgOnSimCard::setDeleteMode(bool value)
{
    if(value)
    {
        m_SimMode = DeleteMode;
    }

    setTitleWithButtons(value);
}

void MsgOnSimCard::setTitleTranslatable()
{
    if(m_SimMode == NormalMode)
    {
        getNaviBar().setTitle(msg("IDS_MSG_HEADER_MESSAGES_ON_SIM_CARD_ABB"));
    }
    else if(m_SimMode == DeleteMode || m_SimMode == CopyToDeviceMode)
    {
        getNaviBar().setTitle(msgArgs("IDS_MSG_HEADER_PD_SELECTED_ABB3", m_CheckCount));
    }
}

void MsgOnSimCard::setTitleWithButtons(bool value)
{
    setTitleTranslatable();

    getNaviBar().showButton(NaviOkButtonId, value);
    getNaviBar().showButton(NaviCancelButtonId, value);

    m_pList->setCheckMode(value);
    m_pList->checkAllItems(false);
    showSelectAllItem(value);
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
        {
            item->setCheckedState(false, true);
        }
    }
    else
    {
        if(item)
        {
            item->destroy();
        }
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
        {
            ++check;
        }
        ++total;
    }
}
