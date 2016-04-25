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

#include "DateLineViewItem.h"
#include "ListView.h"

using namespace Msg;

namespace
{
    ListItemStyleRef dateLineStyle = ListItemStyle::create("dateline");
    const char *datePart = "text.date";
}

DateLineViewItem::DateLineViewItem(const std::string &dateLine)
    : ListItem()
    , m_DateLine(dateLine)
{
    setStyle(dateLineStyle);
}

DateLineViewItem::~DateLineViewItem()
{
}

std::string DateLineViewItem::getDateLine() const
{
    return m_DateLine;
}

void DateLineViewItem::setDateLine(const std::string &dateLine)
{
    m_DateLine = dateLine;
}

std::string DateLineViewItem::getText(ListItem &item, const char *part)
{
    if(!strcmp(part, datePart))
        return m_DateLine;
    else
        return "";
}
