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

#include "SelectAllListItem.h"
#include "Config.h"

#include <string.h>

using namespace Msg;

namespace
{
    ListItemStyleRef style = ListItemStyle::create("type1");
    const char *textPart = "elm.text";
    const char *checkPart = "elm.swallow.end";
}

SelectAllListItem::SelectAllListItem()
    : ListItem(style, ELM_GENLIST_ITEM_NONE, 0)
{
}

SelectAllListItem::~SelectAllListItem()
{
}

const char *SelectAllListItem::getCheckPart(ListItem &item)
{
    return checkPart;
}

std::string SelectAllListItem::getListItemText(ListItem &item, const char *part)
{
    if(strcmp(part, textPart) == 0)
    {
        return msg("IDS_MSG_MBODY_SELECT_ALL");
    }

    return "";
}


