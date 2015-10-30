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
#include "ResourceUtils.h"
#include "ListView.h"
#include "Logger.h"

#include <assert.h>
#include <glib.h>

using namespace Msg;

// TODO: move to common part
std::string searchMarkupKeyword(const std::string &str, const std::string &searchWord)
{
    if(str.empty() || searchWord.empty())
        return str;

    char *found = strcasestr((char*)str.c_str(), (char*)searchWord.c_str());
    if(!found)
        return str;

    std::string res;
    res.reserve(str.length() + 32);

    int diff = found - str.c_str();
    std::string firstPart = std::string(str.begin(), str.begin() + diff);
    std::string lastPart = std::string(str.begin() + diff + searchWord.length(), str.end());

    res += firstPart;
    res += "<match>";
    res += searchWord;
    res += "</match>";
    res += lastPart;

    return res;
}

ContactListItem::ContactListItem(const ContactPersonPhoneLog &rec, const std::string &searchWord)
{
    assert(rec.getAddress());
    setStyle(ContactListItemView::logStyle);

    m_Recipient = rec.getAddress();
    MSG_LOG(m_Recipient);
    m_MainText = searchMarkupKeyword(rec.getAddress(), searchWord);
}

ContactListItem::ContactListItem(const ContactPersonNumber &rec, const std::string &searchWord)
{
    assert(rec.getNumber());
    setStyle(ContactListItemView::nameOrEmailStyle);

    if(rec.getDispName())
        m_MainText = searchMarkupKeyword(rec.getDispName(), searchWord);

    if(rec.getNumber())
    {
        m_SubText = searchMarkupKeyword(rec.getNumber(), searchWord);
        m_Recipient = rec.getNumber();
    }

    if(rec.getThumbnailPath())
        m_ImagePath = rec.getThumbnailPath();
}

ContactListItem::ContactListItem(const ContactPersonEmail &rec, const std::string &searchWord)
{
    assert(rec.getEmail());
    setStyle(ContactListItemView::nameOrEmailStyle);

    if(rec.getDispName())
        m_MainText = searchMarkupKeyword(rec.getDispName(), searchWord);

    if(rec.getEmail())
    {
        m_SubText = searchMarkupKeyword(rec.getEmail(), searchWord);
        m_Recipient = rec.getEmail();
    }

    if(rec.getThumbnailPath())
        m_ImagePath = rec.getThumbnailPath();
}

ContactListItem::~ContactListItem()
{
}

const std::string &ContactListItem::getRecipient() const
{
    MSG_LOG(m_Recipient);
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
    std::string resPath = ResourceUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
    Evas_Object *img = ThumbnailMaker::make(*getOwner(), ThumbnailMaker::MsgType, resPath.c_str());
    return img;
}
