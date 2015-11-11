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

#include "NaviFrameView.h"
#include "Logger.h"

#include <Elementary.h>

using namespace Msg;

namespace
{
    const char *naviTitleStyleEmpty = "empty";
}

NaviFrameView::NaviFrameView(Evas_Object *parent)
    : View()
{
    create(parent);
}

NaviFrameView::NaviFrameView(View &parent)
    : View()
{
    create(parent.getEo());
}

NaviFrameView::~NaviFrameView()
{

}

void NaviFrameView::create(Evas_Object *parent)
{
    setEo(elm_naviframe_add(parent));
    show();
}

bool NaviFrameView::isLastFrame() const
{
    return elm_naviframe_bottom_item_get(getEo()) == elm_naviframe_top_item_get(getEo());
}

void NaviFrameView::push(NaviFrameItem &item, Evas_Object *content)
{
    Elm_Object_Item *it = elm_naviframe_item_push(getEo(), NULL, NULL, NULL, content, NULL);
    elm_naviframe_item_style_set(it, naviTitleStyleEmpty);
    item.setElmObjItem(it);
}

void NaviFrameView::push(NaviFrameItem &item, View &content)
{
    push(item, content.getEo());
}

void NaviFrameView::pop()
{
    elm_naviframe_item_pop(getEo());
}

