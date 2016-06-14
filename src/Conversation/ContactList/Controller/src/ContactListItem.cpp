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

ContactListItem::ContactListItem(const ContactPersonPhoneLog &rec, App &app, const std::string &searchWord)
    : ContactListItem(rec.getAddress(), app)
{
    setStyle(ContactListViewItem::logStyle);

    m_MainText = TextDecorator::highlightKeyword(rec.getAddress(), searchWord);
}

ContactListItem::ContactListItem(const ContactAddress &rec, App &app, const std::string &searchWord)
    : ContactListItem(rec.getAddress(), app)
{
    setStyle(ContactListViewItem::nameOrEmailStyle);

    m_MainText = TextDecorator::highlightKeyword(rec.getDispName(), searchWord);
    m_SubText = TextDecorator::highlightKeyword(rec.getAddress(), searchWord);
}

ContactListItem::ContactListItem(const std::string &recipient, App &app)
    : m_App(app)
    , m_Recipient(recipient)
    , m_ThumbId(m_App.getThumbnailMaker().getThumbId(recipient))
{
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
    return m_App.getThumbnailMaker().getThumbById(*getOwner(), m_ThumbId);
}
