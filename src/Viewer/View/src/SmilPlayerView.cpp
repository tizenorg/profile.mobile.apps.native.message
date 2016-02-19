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

#include "SmilPlayerView.h"

using namespace Msg;

SmilPlayerView::SmilPlayerView(Evas_Object *parent)
{
    setEo(createBox(parent));
}

SmilPlayerView::~SmilPlayerView()
{

}

void SmilPlayerView::displayPage(Evas_Object *page)
{
    Eina_List *list = elm_box_children_get(getEo());
    if(list)
    {
        Eina_List *l = nullptr;
        void *obj = nullptr;

        EINA_LIST_FOREACH(list, l, obj)
        {
            evas_object_hide((Evas_Object*)obj);
        }
        eina_list_free(list);
    }

    elm_box_unpack_all(getEo());
    View::expand(page);
    evas_object_show(page);
    elm_box_pack_start(getEo(), page);
}

Evas_Object *SmilPlayerView::createBox(Evas_Object *parent)
{
    Evas_Object *box = elm_box_add(parent);
    elm_box_align_set(box, 0.5, 0.5);
    elm_box_homogeneous_set(box, false);
    elm_box_align_set(box, 0.0, 0.0);
    return box;
}

