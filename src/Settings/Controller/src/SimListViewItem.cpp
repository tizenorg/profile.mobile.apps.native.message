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
#include "TimeUtils.h"

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

SimListViewItem::SimListViewItem(const MessageSMS &msg, Elm_Genlist_Item_Type type)
    : ListItem(SimItemStyle, type)
{
    updateModel(msg);
}

SimListViewItem::~SimListViewItem()
{
}

void SimListViewItem::updateModel(const MessageSMS &msg)
{
    const MsgAddressList &addressList = msg.getAddressList();
    int length = addressList.getLength();
    for(int i = 0; i < length; ++i)
    {
        m_Name += addressList.at(i).getAddress();
    }

    m_MessageText = msg.getText();
    m_Time = TimeUtils::makeDateTimeString(msg.getTime());
    m_MsgId = msg.getId();
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
    return m_Name;
}

std::string SimListViewItem::getMessage() const
{
    return m_MessageText + "<br/><br/>" + m_Time;
}

MsgId SimListViewItem::getMsgId() const
{
    return m_MsgId;
}
