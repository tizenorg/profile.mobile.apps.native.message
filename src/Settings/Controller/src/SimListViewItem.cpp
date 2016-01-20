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

#include "SimListViewItem.h"
#include "Logger.h"
#include "PathUtils.h"
#include "ThumbnailMaker.h"
#include "TextDecorator.h"

#include <Elementary.h>

using namespace Msg;

namespace
{
    ListItemStyleRef SimItemStyle = ListItemStyle::create("multiline");

    const char *messagePart = "elm.text.multiline";
    const char *namePart = "elm.text";
    const char *checkBoxPart = "elm.swallow.end";

    const TextStyle nameTextStyle(40, "#131313FF");
    const TextStyle msgTextStyle(32, "#969696FF");
}

SimListViewItem::SimListViewItem(Elm_Genlist_Item_Type type)
    : ListItem(SimItemStyle, type)
{
}

SimListViewItem::~SimListViewItem()
{
}

std::string SimListViewItem::getText(ListItem &item, const char *part)
{
    if(!strcmp(part, messagePart))
    {
        return TextDecorator::make(getMessage(), msgTextStyle);
    }
    else if(!strcmp(part, namePart))
    {
        return TextDecorator::make(getName(), nameTextStyle);
    }

    return "";
}

const char *SimListViewItem::getCheckPart(ListItem &item)
{
    return checkBoxPart;
}

std::string SimListViewItem::getName() const
{
    std::string number = "0936403503";// TODO: remove hardcode
    return number;
}

std::string SimListViewItem::getMessage() const
{
    std::string message = "Hello, world!";// TODO: remove hardcode
    std::string time = getTime();
    message = message + "<br/><br/>" + time;
    return message;
}

std::string SimListViewItem::getTime() const
{
    std::string time = "12:00 AM";// TODO: remove hardcode
    return time;
}
