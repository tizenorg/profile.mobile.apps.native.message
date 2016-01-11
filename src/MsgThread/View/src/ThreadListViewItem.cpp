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

#include "ThreadListViewItem.h"
#include "Logger.h"
#include "PathUtils.h"
#include "ThumbnailMaker.h"
#include "TextDecorator.h"

#include <Elementary.h>
#include <stdlib.h>

using namespace Msg;

namespace
{
    ListItemStyleRef threadItemStyle = ListItemStyle::create("type2");

    const char *messagePart = "elm.text";
    const char *namePart = "elm.text.sub";
    const char *timePart = "elm.text.sub.end";
    const char *thumbnailPart = "elm.swallow.icon";
    const char *checkBoxPart = "elm.swallow.end";
    const char *statusPart = "elm.text.end";
    const char *iconPart = "elm.swallow.icon.1";

    const TextStyle nameTextStyle(44, "#131313FF");
    const TextStyle msgTextStyle(38, "#969696FF");
    const TextStyle statusTextStyle(36, "#e43d3dFF");
    const TextStyle timeTextStyle(36, "#969696FF");
    const TextStyle unreadTextStyle(36, "#969696FF");
}

ThreadListViewItem::ThreadListViewItem(Elm_Genlist_Item_Type type)
    : ListItem(threadItemStyle, type)
    , m_State(NormalState)
{
}

ThreadListViewItem::~ThreadListViewItem()
{
}

std::string ThreadListViewItem::getListItemText(ListItem &item, const char *part)
{
    if(!strcmp(part, messagePart))
    {
        return TextDecorator::make(getMessage(), msgTextStyle);
    }
    else if(!strcmp(part, namePart))
    {
        return TextDecorator::make(getName(), nameTextStyle);
    }
    else if(!strcmp(part, timePart))
    {
        return TextDecorator::make(getTime(), timeTextStyle);
    }
    else if(!strcmp(part, statusPart))
    {
        if(m_State == StatusState)
        {
            return TextDecorator::make(getStatus(), statusTextStyle);
        }
    }

    return "";
}

Evas_Object *ThreadListViewItem::getListItemContent(ListItem &item, const char *part)
{
    Evas_Object *content = nullptr;

    if(!strcmp(part, thumbnailPart))
    {
        content = getThumbnail();
    }
    else if(!strcmp(part, iconPart))
    {
        if(m_State == IconState)
        {
            content = getIcon();
        }
    }

    return content;
}

const char *ThreadListViewItem::getCheckPart(ListItem &item)
{
    return checkBoxPart;
}

void ThreadListViewItem::setState(State state, bool updateUi)
{
    m_State = state;
    if(updateUi)
    {
        updateFields(iconPart, ELM_GENLIST_ITEM_FIELD_CONTENT);
        updateFields(statusPart, ELM_GENLIST_ITEM_FIELD_TEXT);
    }
}

ThreadListViewItem::State ThreadListViewItem::getState() const
{
    return m_State;
}

Evas_Object *ThreadListViewItem::makeUnreadIcon(Evas_Object *parent, const std::string &text)
{
    Evas_Object *label = elm_label_add(parent);
    evas_object_show(label);
    std::string decorText = TextDecorator::make(text, unreadTextStyle);
    elm_object_text_set(label, decorText.c_str());
    return label;
}

std::string ThreadListViewItem::getStatus()
{
    return std::string();
}

Evas_Object *ThreadListViewItem::getIcon()
{
    return nullptr;
}

