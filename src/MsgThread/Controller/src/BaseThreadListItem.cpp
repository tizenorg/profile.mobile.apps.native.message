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
#include "BaseThreadListItem.h"
#include "ContactManager.h"
#include "Resource.h"
#include "ListView.h"

using namespace Msg;

BaseThreadListItem::BaseThreadListItem(App &app)
    : m_App(app)
    , m_ThumbType(ThumbnailMaker::UserType)
{

}

BaseThreadListItem::~BaseThreadListItem()
{

}

void BaseThreadListItem::updateThumbnail(const MsgThreadItem &threadItem)
{
    const MsgAddressListRef addressList = m_App.getMsgEngine().getStorage().getAddressList(threadItem.getId());
    if(addressList)
        updateThumbnail(*addressList);
}

void BaseThreadListItem::updateThumbnail(const MsgAddressList &addressList)
{
    int countContact = addressList.getLength();
    m_ThumbType = ThumbnailMaker::MsgType;
    if(countContact > 1)
    {
        m_ThumbPath = PathUtils::getResourcePath(THUMB_GROUP_IMG_PATH);
    }
    else if(countContact == 1)
    {
        const MsgAddress &addr = addressList[0];
        updateThumbnail(addr);
    }
}

void BaseThreadListItem::updateThumbnail(const MsgAddress &addr)
{
    ContactPersonNumberRef contactNumber = m_App.getContactManager().getContactPersonNumber(addr.getAddress());

    if(contactNumber)
        m_ThumbPath = contactNumber->getThumbnailPath();
    if(m_ThumbPath.empty())
        m_ThumbPath = PathUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
    else
        m_ThumbType = ThumbnailMaker::UserType;
}

void BaseThreadListItem::updateTime(time_t time)
{
    m_Time = decorateTimeText(std::string("7:40")); // TODO: remove hardcode
}

std::string BaseThreadListItem::getName()
{
    return m_Name;
}

std::string BaseThreadListItem::getMessage()
{
    return m_Message;
}

std::string BaseThreadListItem::getTime()
{
    return m_Time;
}

Evas_Object *BaseThreadListItem::getThumbnail()
{
    return ThumbnailMaker::make(*getOwner(), m_ThumbType, m_ThumbPath);
}
