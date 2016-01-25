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


#include "ContactListItem.h"
#include "ThumbnailMaker.h"
#include "PathUtils.h"
#include "ListView.h"
#include "Logger.h"
#include "TextDecorator.h"

#include <assert.h>

using namespace Msg;

ContactListItem::ContactListItem(const ContactPersonPhoneLog &rec, const std::string &searchWord)
{
    setStyle(ContactListViewItem::logStyle);

    m_Recipient = rec.getAddress();
    MSG_LOG(m_Recipient);
    m_MainText = TextDecorator::highlightKeyword(rec.getAddress(), searchWord);
}

ContactListItem::ContactListItem(const ContactPersonNumber &rec, const std::string &searchWord)
{
    setStyle(ContactListViewItem::nameOrEmailStyle);

    m_MainText = TextDecorator::highlightKeyword(rec.getDispName(), searchWord);
    m_SubText = TextDecorator::highlightKeyword(rec.getNumber(), searchWord);
    m_Recipient = rec.getNumber();
    m_ImagePath = rec.getThumbnailPath();
}

ContactListItem::ContactListItem(const ContactPersonEmail &rec, const std::string &searchWord)
{
    setStyle(ContactListViewItem::nameOrEmailStyle);

    m_MainText = TextDecorator::highlightKeyword(rec.getDispName(), searchWord);
    m_SubText = TextDecorator::highlightKeyword(rec.getEmail(), searchWord);
    m_Recipient = rec.getEmail();
    m_ImagePath = rec.getThumbnailPath();
}

ContactListItem::~ContactListItem()
{
}

const std::string &ContactListItem::getRecipient() const
{
    return m_Recipient;
}

std::string ContactListItem::getSubText() const
{
    return m_SubText;
}

std::string ContactListItem::getMainText() const
{
    return m_MainText;
}

Evas_Object *ContactListItem::getThumbnail() const
{
    std::string resPath = PathUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
    Evas_Object *img = ThumbnailMaker::make(*getOwner(), ThumbnailMaker::MsgType, resPath.c_str());
    return img;
}
