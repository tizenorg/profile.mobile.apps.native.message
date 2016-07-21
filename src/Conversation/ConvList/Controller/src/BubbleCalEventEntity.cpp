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

#include "BubbleCalEventEntity.h"
#include "VCalendarParser.h"
#include "Resource.h"

using namespace Msg;


BubbleCalEventEntity::BubbleCalEventEntity(const MsgConvMedia &convMedia)
    : BubbleEntity(CalendarEventItem)
    , m_FilePath(convMedia.getPath())
{
    auto list = VCalendarParser::getInst().parse(convMedia.getPath());
    if(list.size() == 1)
    {
        const CalendarEvent &event = list.front();
        m_Name = event.getSummary();
        m_DateTime = event.getStartDate();
    }
    else
    {
        m_Name = getFileName(convMedia);
    }
}

BubbleCalEventEntity::~BubbleCalEventEntity()
{
}

BubbleCalEventViewItem *BubbleCalEventEntity::createView(Evas_Object *parent)
{
    BubbleCalEventViewItem::LayoutType type = m_DateTime.empty() ? BubbleCalEventViewItem::Layout1Icon1Text :
                                                                   BubbleCalEventViewItem::Layout1Icon2Text;
    auto *item = new BubbleCalEventViewItem(*this, parent, type);
    item->setMainText(m_Name);
    if(!m_DateTime.empty())
        item->setSubText(m_DateTime);
    return item;
}

std::string BubbleCalEventEntity::getFilePath() const
{
    return m_FilePath;
}
