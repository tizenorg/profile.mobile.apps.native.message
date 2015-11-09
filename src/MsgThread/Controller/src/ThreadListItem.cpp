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
#include "MsgEngine.h"
#include "App.h"
#include "MsgThreadItem.h"

#include <Elementary.h>

using namespace Msg;

ThreadListItem::ThreadListItem(const MsgThreadItem &threadItem, App &app)
    : ThreadListViewItem()
    , m_ThreadId(threadItem.getId())
    , m_App(app)
    , m_ThumbType(ThumbnailMaker::MsgType)
{
    setState(IconState, false);
    setCheckedState(false, false);
    updateModel(threadItem);
}

ThreadListItem::~ThreadListItem()
{
}

ThreadListViewItem &ThreadListItem::getViewItem()
{
    return *this;
}

ThreadId ThreadListItem::getThreadId() const
{
    return m_ThreadId;
}

std::string ThreadListItem::getName()
{
    return m_Name;
}

std::string ThreadListItem::getMessage()
{
    return m_Message;
}

Evas_Object *ThreadListItem::getThumbnail()
{
    return ThumbnailMaker::make(*getOwner(), m_ThumbType, m_ThumbPath);
}

void ThreadListItem::updateThumbnail(const MsgThreadItem &threadItem)
{
    const MsgAddressListRef addressList = m_App.getMsgEngine().getStorage().getAddressList(threadItem.getId());

    if(addressList)
    {
        int countContact = addressList->getLength();
        m_ThumbType = ThumbnailMaker::MsgType;
        if(countContact > 1)
        {
            m_ThumbPath = ResourceUtils::getResourcePath(THUMB_GROUP_IMG_PATH);
        }
        else if(countContact == 1)
        {
            const MsgAddress &addr = addressList->at(0);
            const char *thumbPath = m_App.getContactManager().getContactPersonNumber(addr.getAddress()).getThumbnailPath();
            if(thumbPath)
            {
                m_ThumbPath.assign(thumbPath);
                m_ThumbType = ThumbnailMaker::UserType;
            }
            else
            {
                m_ThumbPath = ResourceUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
            }
        }
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

void ThreadListItem::updateModel(const MsgThreadItem &threadItem)
{
    m_ThreadId = threadItem.getId();
    m_Message = threadItem.getLastMessage();
    m_Name = threadItem.getName();

    updateThumbnail(threadItem);
}

