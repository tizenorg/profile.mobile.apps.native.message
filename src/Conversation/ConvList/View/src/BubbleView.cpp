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

#include "BubbleView.h"

#include <Elementary.h>

using namespace Msg;

const char *bubbleSentStyle = "sentmessage/custom/sent_style_01";
const char *bubbleRecvStyle = "readmessage/custom/recv_style_01";

BubbleView::BubbleView(Evas_Object *parent, Style style)
    : m_Style(style)
{
    create(parent);
}

BubbleView::~BubbleView()
{

}

const char *BubbleView::getStyle() const
{
    switch(m_Style)
    {
        case Sent:
            return bubbleSentStyle;
            break;

        case Received:
            return bubbleRecvStyle;
            break;

        default:
            break;
    }

    return nullptr;
}

void BubbleView::create(Evas_Object *parent)
{
    Evas_Object *label = elm_label_add(parent);
    setEo(label);
    evas_object_show(label);
    elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    elm_object_focus_allow_set(label, true);
    elm_object_tree_focus_allow_set(label, true);
    elm_object_style_set(label, getStyle());
    evas_object_color_set(label, rand() % 230, rand() % 230, rand() % 230, 190);

   // edje_object_signal_emit(elm_layout_edje_get(label), "elm,state,text,status,enabled", "");
   // edje_object_signal_emit(elm_layout_edje_get(label), "elm,state,effect,show", "");
   // edje_object_signal_emit(elm_layout_edje_get(label), "elm,state,contents,enabled", "");

    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);

    // Demo Colors
    if(m_Style == Received)
    {
        evas_object_color_set(label, rand() % 230, rand() % 230, 255, 190);
    }
    else
    {
        evas_object_color_set(label, 255, rand() % 230, rand() % 230, 190);
    }
    //Demo colors
}

void BubbleView::setText(const std::string &text)
{
    elm_object_text_set(getEo(), text.c_str());
}

void BubbleView::setTime(const std::string &time)
{
    elm_object_part_text_set(getEo(), "elm.text.time", time.c_str());
}
