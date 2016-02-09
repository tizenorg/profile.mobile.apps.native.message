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
#include "Logger.h"
#include "TimeUtils.h"

using namespace Msg;

BaseThreadListItem::BaseThreadListItem(App &app)
    : m_App(app)
    , m_ThumbType(ThumbnailMaker::UserType)
{

}

BaseThreadListItem::~BaseThreadListItem()
{

}

void BaseThreadListItem::updateMessage(const MsgThreadItem &threadItem)
{
    updateMessage(threadItem.getLastMessage());
}

void BaseThreadListItem::updateMessage(const std::string &msg)
{
    m_Message = decorateMessageText(msg);
}

void BaseThreadListItem::updateThumbnailAndName(const MsgThreadItem &threadItem, bool decorateName)
{
    const MsgAddressListRef addressList = m_App.getMsgEngine().getStorage().getAddressList(threadItem.getId());
    if(addressList)
        updateThumbnailAndName(*addressList, decorateName);
}

void BaseThreadListItem::updateThumbnailAndName(const MsgAddressList &addressList, bool decorateName)
{
    int countContact = addressList.getLength();
    const MsgAddress &addr = addressList[0];

    m_ThumbType = ThumbnailMaker::MsgType;
    if(countContact > 1)
    {
        m_ThumbPath = PathUtils::getResourcePath(THUMB_GROUP_IMG_PATH);
        updateName(addr, countContact, decorateName);
    }
    else if(countContact == 1)
    {
        updateThumbnailAndName(addr, countContact, decorateName);
    }
    else
    {
        MSG_LOG_WARN("Msg address list is empty");
    }
}

void BaseThreadListItem::updateThumbnailAndName(const MsgAddress &addr, int addressesCount, bool decorateName)
{
    ContactPersonAddressRef contactAddress = m_App.getContactManager().getContactPersonAddress(addr.getAddress());

    if(contactAddress)
    {
        m_ThumbPath = contactAddress->getThumbnailPath();
        updateName(*contactAddress, addressesCount, decorateName);
    }
    else
    {
        updateName(addr, addressesCount, decorateName);
    }
    if(m_ThumbPath.empty())
        m_ThumbPath = PathUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
    else
        m_ThumbType = ThumbnailMaker::UserType;
}

void BaseThreadListItem::updateName(const MsgAddress &address, int addressesCount, bool decorateName)
{
    ContactPersonAddressRef contactAddress = m_App.getContactManager().getContactPersonAddress(address.getAddress());
    if(contactAddress)
        updateName(*contactAddress, addressesCount, decorateName);
    else
        updateName(address.getAddress(), addressesCount, decorateName);
}

void BaseThreadListItem::updateName(const ContactPersonAddress &address, int addressesCount, bool decorateName)
{
    std::string dispName = address.getDispName();
    if(dispName.empty())
        dispName = address.getAddress();
    updateName(dispName, addressesCount, decorateName);
}

void BaseThreadListItem::updateName(const std::string &address, int addressesCount, bool decorateName)
{
    m_Name = address;
    --addressesCount;
    if(addressesCount > 0)
        m_Name += " +" + std::to_string(addressesCount);
    if(decorateName)
        m_Name = decorateNameText(m_Name);
}

void BaseThreadListItem::updateTime(time_t time)
{
    m_Time = TimeUtils::makeThreadTimeString(time);
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
