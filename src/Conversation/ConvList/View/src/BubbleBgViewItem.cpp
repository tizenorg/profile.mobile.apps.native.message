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

#include "BubbleBgViewItem.h"
#include "Resource.h"

using namespace Msg;

BubbleBgViewItem::BubbleBgViewItem(BubbleEntity &entity, Evas_Object *parent, BgType bgType)
    : BubbleViewItem(entity)
{
    const char *group = nullptr;
    switch(bgType)
    {
        case SentStyle:
            group = "conv/list/bubble_sent_bg";
            break;
        case ReceivedStyle:
            group = "conv/list/bubble_received_bg";
            break;
        case DraftStyle:
            group = "conv/list/bubble_draft_bg";
            break;
        case FailedStyle:
            group = "conv/list/bubble_failed_bg";
            break;
    }

    setEo(addLayout(parent, CONV_LIST_BUBBLE_EDJ_PATH, group));
    evas_object_size_hint_align_set(getEo(), 1.0, EVAS_HINT_FILL);
    attachGestureTapLayer(getEo(), getEo());
}

BubbleBgViewItem::~BubbleBgViewItem()
{

}

void BubbleBgViewItem::setContent(Evas_Object *obj)
{
    View::setContent(obj, "content");
}

Evas_Object *BubbleBgViewItem::getContent() const
{
    return View::getContent("content");
}

void BubbleBgViewItem::showSearch(bool search)
{
    // TODO: impl
}
