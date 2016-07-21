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

using namespace Msg;

namespace
{
    const char *textStyle = "DEFAULT='font=Tizen:style=Regular font_size=24 wrap=mixed text_class=label'";
}

BubbleTextViewItem::BubbleTextViewItem(BubbleEntity &entity, Evas_Object *parent, const std::string &text)
    : BubbleViewItem(entity)
{
    setEo(createText(parent, text));
}

BubbleTextViewItem::~BubbleTextViewItem()
{
}

Evas_Object *BubbleTextViewItem::createText(Evas_Object *parent, const std::string &text)
{
    //TODO: apply to label the same text style as to textblock (figure out how-to)
    if(text.empty())
        return nullptr;

    Evas_Coord ww = 0;
    Evas_Coord hh = 0;

    Evas_Object *label = elm_label_add(parent);
    Evas_Object *textBlock = evas_object_textblock_add(evas_object_evas_get(label));
    Evas_Textblock_Style *ts = evas_textblock_style_new();

    evas_textblock_style_set(ts, textStyle);
    evas_object_textblock_style_set(textBlock, ts);
    evas_object_textblock_text_markup_set(textBlock, text.c_str());
    evas_object_textblock_size_native_get(textBlock, &ww, &hh);

    evas_textblock_style_free(ts);
    evas_object_del(textBlock);

    int wrapWidth = ww > maxWidth ? (int)ELM_SCALE_SIZE(maxWidth) : (int)ELM_SCALE_SIZE(ww);
    elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    elm_label_wrap_width_set(label, wrapWidth);
    elm_object_part_text_set(label, nullptr, text.c_str());
    evas_object_show(label);
    evas_object_size_hint_align_set(label, 0.0, EVAS_HINT_FILL);
    return label;
}
