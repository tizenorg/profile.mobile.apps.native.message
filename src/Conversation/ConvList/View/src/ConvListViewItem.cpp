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

#include "ConvListViewItem.h"

using namespace Msg;

namespace
{
    //TODO: Implement different styles
    ListItemStyleRef sentStyle = ListItemStyle::create("sentbubble");
    ListItemStyleRef receivedStyle = ListItemStyle::create("receivebubble");
    ListItemStyleRef draftStyle = ListItemStyle::create("draft");

    const char *checkBoxPart = "bubble.check";
    const char *bubbleContentPart = "bubble.content";
    const char *thumbContentPart = "info.thumbnail";
    const char *timeTextPart = "info.time";
}

ConvListViewItem::ConvListViewItem(ConvItemType type)
    : ListItem()
{
    switch (type)
    {
        case Sent:
            setStyle(sentStyle);
            break;
        case Received:
            setStyle(receivedStyle);
            break;
        case Draft:
            setStyle(draftStyle);
            break;
        default:
            break;
    }
}

ConvListViewItem::~ConvListViewItem()
{

}

std::string ConvListViewItem::getText(ListItem &item, const char *part)
{
    if(!strcmp(part, "elm.text"))
        return getText();
    else if(!strcmp(part, timeTextPart))
        return getTime();
    else
        return "";
}

Evas_Object *ConvListViewItem::getContent(ListItem &item, const char *part)
{
    if(!strcmp(part, bubbleContentPart))
        return getBubble();
    else if(!strcmp(part, thumbContentPart))
        return getThumbnail();
    else
        return nullptr;
}

const char *ConvListViewItem::getCheckPart(ListItem &item)
{
    return checkBoxPart;
}
