/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "PopupListItem.h"

using namespace Msg;

const ListItemStyleRef listItemStyle = ListItemStyle::create("type1");

PopupListItem::PopupListItem(PopupList &parent, PopupListItemPressedCb cb, void *userData)
    : ListItem(listItemStyle)
    , m_Parent(parent)
    , m_Cb(cb)
    , m_pUserData(userData)
{
}

void PopupListItem::fireCallback()
{
    if (m_Cb)
        m_Cb(*this, m_pUserData);
}

PopupList &PopupListItem::getParent()
{
    return m_Parent;
}

PopupTextListItem::PopupTextListItem(PopupList &parent, const std::string &text, PopupListItemPressedCb cb, void *userData)
    : PopupListItem(parent, cb, userData)
    , m_Text(text)
{
}

PopupTextListItem::~PopupTextListItem()
{
}

std::string PopupTextListItem::getText(ListItem &item, const char *part)
{
    if(strcmp(part, "elm.text") == 0)
        return m_Text;
    return std::string();
}

PopupCheckListItem::PopupCheckListItem(PopupList &parent, const std::string &text, PopupListItemPressedCb cb, void *userData)
    : PopupListItem(parent, cb, userData)
    , m_Text(text)
{
}

PopupCheckListItem::~PopupCheckListItem()
{
}

const char *PopupCheckListItem::getCheckPart(ListItem &item)
{
    return "elm.swallow.end";
}

std::string PopupCheckListItem::getText(ListItem &item, const char *part)
{
    if(strcmp(part, "elm.text") == 0)
        return m_Text;
    return std::string();
}
