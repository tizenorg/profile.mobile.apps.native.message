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

#include "ThreadListItem.h"
#include "ViewController.h"
#include "ListView.h"
#include "Logger.h"
#include "ResourceUtils.h"
#include "ThumbnailMaker.h"
#include "MsgEngine.h"
#include "App.h"
#include "MsgThreadItem.h"

#include <Elementary.h>

using namespace Msg;

ThreadListItem::ThreadListItem(BaseMsgThreadItemRef threadItem, App &app)
    : m_pMsgThreadItem(threadItem)
    , m_App(app)
{
    MSG_LOG("Constructor");
    setState(IconState, false);
    setCheckedState(false, false);
}

ThreadListItem::~ThreadListItem()
{
    MSG_LOG("Destructor");
}

ThreadListViewItem &ThreadListItem::getViewItem()
{
    return *this;
}

BaseMsgThreadItemRef ThreadListItem::getModel() const
{
    return m_pMsgThreadItem;
}

std::string ThreadListItem::getName()
{
    return m_pMsgThreadItem->getName();
}

std::string ThreadListItem::getMessage()
{
    return m_pMsgThreadItem->getLastMessage();
}

Evas_Object *ThreadListItem::getThumbnail()
{
    AddressList addressList = m_App.getMsgEngine().getStorage().getAddressListByThreadId(m_pMsgThreadItem->getId());
    int count = addressList.size();
    std::string resPath;
    ThumbnailMaker::Type type = ThumbnailMaker::MsgType;

    if(count > 1)
    {
        resPath = ResourceUtils::getResourcePath(THUMB_GROUP_IMG_PATH);
    }
    else if(count == 1)
    {
        resPath = updateThumbnail(addressList[0]);
        if(resPath.empty())
        {
            resPath = ResourceUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
        }
        else
        {
            type = ThumbnailMaker::UserType;
        }
    }
    Evas_Object *img = ThumbnailMaker::make(*getOwner(), type, resPath.c_str());

    return img;
}

std::string ThreadListItem::updateThumbnail(const std::string &number)
{
    ContactPersonNumber contactPersonNumber = m_App.getContactManager().getContactPersonNumber(number);
    const char *str = contactPersonNumber.getThumbnailPath();
    if(str)
    {
        return str;
    }
    else
    {
        return "";
    }
}

std::string ThreadListItem::getTime()
{
    //m_MsgThreadItem.getTime();
    return std::string("7:40"); // TODO: remove hardcode
}

std::string ThreadListItem::getStatus()
{
    //m_MsgThreadItem.getStatus()
    return "normal"; // TODO: remove hardcode
}

Evas_Object *ThreadListItem::getIcon()
{
    Evas_Object *img = makeUnreadBadge(4); // TODO: remove hardcode
    return img;
}

Evas_Object *ThreadListItem::makeUnreadBadge(int unreadCount)
{
    Evas_Object *label = makeUnreadIcon(*getOwner() ,std::to_string(unreadCount));
    return label;
}



