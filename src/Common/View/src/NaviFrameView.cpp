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

int NaviFrameView::getItemsCount() const
{
    int res = 0;
    Eina_List *list= elm_naviframe_items_get(getEo());
    if(list)
    {
        res = eina_list_count(list);
        eina_list_free(list);
    }
    return res;
}

NaviFrameItemList NaviFrameView::getItems() const
{
    NaviFrameItemList result;
    Eina_List *list= elm_naviframe_items_get(getEo());

    if(list)
    {
        Eina_List *l = nullptr;
        void *obj = nullptr;

        EINA_LIST_FOREACH(list, l, obj)
        {
            NaviFrameItem *item = static_cast<NaviFrameItem*>(elm_object_item_data_get((Elm_Object_Item*)obj));
            result.push_back(item);
        }

        eina_list_free(list);
    }
    return result;
}

bool NaviFrameView::isEmpty() const
{
    return getItemsCount() <= 0;
}

void NaviFrameView::push(NaviFrameItem &item, Evas_Object *content)
{
    Elm_Object_Item *it = elm_naviframe_item_push(getEo(), nullptr, nullptr, nullptr, content, nullptr);
    item.setElmObjItem(it);
}

void NaviFrameView::push(NaviFrameItem &item, View &content)
{
    push(item, content);
}

void NaviFrameView::pop()
{
    elm_naviframe_item_pop(getEo());
}

void NaviFrameView::insertToBottom(NaviFrameItem &item)
{
    Elm_Object_Item *before = elm_naviframe_bottom_item_get(getEo());
    Elm_Object_Item *insert = nullptr;

    if(before)
        insert = elm_naviframe_item_insert_before(getEo(), before, nullptr, nullptr, nullptr, nullptr, nullptr);
    else
        insert = elm_naviframe_item_push(getEo(), nullptr, nullptr, nullptr, nullptr, nullptr);

    item.setElmObjItem(insert);
}

void NaviFrameView::promote(NaviFrameItem &item)
{
    elm_naviframe_item_promote(item);
}
