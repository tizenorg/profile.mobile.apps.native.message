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

#include "ContactListViewItem.h"
#include <string.h>

using namespace Msg;

ListItemStyleRef ContactListViewItem::logStyle = ListItemStyle::create("1line");
ListItemStyleRef ContactListViewItem::nameOrEmailStyle = ListItemStyle::create("2line.top");

ContactListViewItem::ContactListViewItem()
    : ListItem(ELM_GENLIST_ITEM_NONE)
{
}

ContactListViewItem::~ContactListViewItem()
{
}

std::string ContactListViewItem::getText(ListItem &item, const char *part)
{
    if(getStyle() == nameOrEmailStyle)
    {
        const char *mainTextPart = "elm.text.main.left.top";
        const char *subTextPart = "elm.text.sub.left.bottom";

        if(strcmp(part, mainTextPart) == 0)
        {
            return getMainText();
        }
        else if(strcmp(part, subTextPart) == 0)
        {
            return getSubText();
        }
    }
    if(getStyle() == logStyle)
    {
        const char *mainTextPart = "elm.text.main.left";

        if(strcmp(part, mainTextPart) == 0)
        {
            return getMainText();
        }
    }

    return "";
}

Evas_Object *ContactListViewItem::getContent(ListItem &item, const char *part)
{
    if(strcmp(part, "elm.icon.1") == 0)
    {
        return getThumbnail();
    }
    return nullptr;
}
