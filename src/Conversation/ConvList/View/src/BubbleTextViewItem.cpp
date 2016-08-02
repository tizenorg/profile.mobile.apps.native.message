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

#include "BubbleTextViewItem.h"
#include "Resource.h"
#include "Logger.h"

using namespace Msg;

BubbleTextViewItem::BubbleTextViewItem(BubbleEntity &entity, Evas_Object *parent, BgType bgType, const std::string &text)
    : BubbleBgViewItem(entity, parent, bgType)
{
    setContent(createText(*this, applyColor(text)));
}

BubbleTextViewItem::~BubbleTextViewItem()
{
}

Evas_Object *BubbleTextViewItem::createText(Evas_Object *parent, const std::string &text)
{
    if(text.empty())
        return nullptr;

    Evas_Object *label = elm_label_add(parent);
    elm_object_style_set(label, "conv_bubble");

    elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    elm_object_part_text_set(label, nullptr, text.c_str());
    evas_object_show(label);

    Evas_Object *edjeLabel = elm_layout_edje_get(label);
    const Evas_Object *textBlock = edje_object_part_object_get(edjeLabel, "elm.text");

    Evas_Coord ww = 0;
    Evas_Coord hh = 0;
    evas_object_textblock_size_native_get(textBlock, &ww, &hh);
    int wrapWidth = ww > maxWidth ? maxWidth : ww;
    elm_label_wrap_width_set(label, wrapWidth);

    return label;
}
