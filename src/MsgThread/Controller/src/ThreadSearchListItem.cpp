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

#include "ThreadSearchListItem.h"
#include "ListView.h"
#include "Logger.h"
#include "PathUtils.h"
#include "MsgEngine.h"
#include "LangUtils.h"
#include "ContactManager.h"
#include "TextDecorator.h"

using namespace Msg;

ThreadSearchListItem::ThreadSearchListItem(const Message &msg, App &app, const std::string &searchWord)
    : m_MsgId(msg.getId())
    , m_App(app)
    , m_ThumbType(ThumbnailMaker::MsgType)
{
    setState(NormalState, false);
    updateModel(msg, searchWord);
}

ThreadSearchListItem::~ThreadSearchListItem()
{
}

MsgId ThreadSearchListItem::getMsgId() const
{
    return m_MsgId;
}

std::string ThreadSearchListItem::getName()
{
    return m_Name;
}

std::string ThreadSearchListItem::getMessage()
{
    return m_Message;
}

Evas_Object *ThreadSearchListItem::getThumbnail()
{
    return ThumbnailMaker::make(*getOwner(), m_ThumbType, m_ThumbPath);
}

void ThreadSearchListItem::updateThumbnail(const Message &msg)
{
    const MsgAddressList &addressList = msg.getAddressList();
    int countContact = addressList.getLength();
    m_ThumbType = ThumbnailMaker::MsgType;

    if(countContact > 1)
    {
        m_ThumbPath = PathUtils::getResourcePath(THUMB_GROUP_IMG_PATH);
    }
    else if(countContact == 1)
    {
        const MsgAddress &addr = addressList.at(0);
        ContactPersonNumber contactNumber = m_App.getContactManager().getContactPersonNumber(addr.getAddress());
        const char *thumbPath = contactNumber.isValid() ? contactNumber.getThumbnailPath() : nullptr;
        if(thumbPath)
        {
            m_ThumbPath.assign(thumbPath);
            m_ThumbType = ThumbnailMaker::UserType;
        }
        else
        {
            m_ThumbPath = PathUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
        }
        contactNumber.release();
    }
}

std::string ThreadSearchListItem::getTime()
{
    //m_MsgThreadItem.getTime();
    return std::string("7:40"); // TODO: remove hardcode
}

Evas_Object *ThreadSearchListItem::makeUnreadBadge(int unreadCount)
{
    Evas_Object *label = makeUnreadIcon(*getOwner() ,std::to_string(unreadCount));
    return label;
}

void ThreadSearchListItem::updateModel(const Message &msg, const std::string &searchWord)
{
    m_MsgId = msg.getId();
    m_Message = TextDecorator::highlightKeyword(msg.getText(), searchWord);

    const MsgAddressList &addressList = msg.getAddressList();
    int addrCount = addressList.getLength();
    if(addrCount > 0)
    {
        m_Name = TextDecorator::highlightKeyword(addressList.at(0).getAddress(), searchWord);
        if(addrCount > 1)
            m_Name += " + " + std::to_string(addrCount - 1);
    }

    updateThumbnail(msg);
}

