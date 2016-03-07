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

#include "Page.h"
#include "PageView.h"
#include "Body.h"
#include "Logger.h"
#include "MsgEngine.h"
#include "TextPageViewItem.h"
#include "MediaPageViewItem.h"

using namespace Msg;

MsgTextMetric::MsgTextMetric()
    : charsLeft(0)
    , segmentsCount(0)
    , bytes(0)
    , isMms(false)
{
}

void MsgTextMetric::reset()
{
    charsLeft = 0;
    segmentsCount = 0;
    bytes = 0;
    isMms = false;
}

Page::Page(Body &parent)
    : PageView(parent)
    , m_MsgMetric()
{
}

Page::~Page()
{
}

const MsgTextMetric &Page::getTextMetric()
{
    updateMsgMetricIfNeeded();
    return m_MsgMetric;
}

void Page::updateMsgMetricIfNeeded()
{
    TextPageViewItem *item = static_cast<TextPageViewItem*>(getItem(PageViewItem::TextType));
    if(item)
    {
        if(item->hasChanged())
        {
            std::string text = item->getPlainUtf8Text();
            MsgEngine::calculateTextMetric(text, m_MsgMetric.charsLeft, m_MsgMetric.segmentsCount, m_MsgMetric.bytes, m_MsgMetric.isMms);
            item->resetChangedFlag();
        }
    }
    else
    {
        m_MsgMetric.reset();
    }
}

long long Page::getSize()
{
    long long totalSize = 0;
    auto items = getItems();
    for(PageViewItem *item : items)
    {
        MediaPageViewItem *mediaItem = dynamic_cast<MediaPageViewItem*>(item);
        if(mediaItem)
        {
            int size = mediaItem->getFileSize();
            if(size > 0)
                totalSize += size;
        }
    }
    updateMsgMetricIfNeeded();
    totalSize += m_MsgMetric.bytes;

    return totalSize;
}

bool Page::isMms()
{
    auto pageItems = getItems();
    for(PageViewItem *pageItem : pageItems)
    {
        PageViewItem::Type itemType = pageItem->getType();
        if(itemType != PageViewItem::TextType)
            return true;
    }

    updateMsgMetricIfNeeded();
    return m_MsgMetric.isMms;
}
