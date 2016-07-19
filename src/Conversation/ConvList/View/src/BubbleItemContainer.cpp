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

#include "BubbleItemContainer.h"

using namespace Msg;

namespace
{
    const int verticalBoxPads = 10;
    const int horizontalBoxPads = 0;
}

BubbleItemContainer::BubbleItemContainer(Evas_Object *parent)
{
    Evas_Object *box = elm_box_add(parent);
    elm_box_homogeneous_set(box, false);
    elm_box_padding_set(box, ELM_SCALE_SIZE(horizontalBoxPads), ELM_SCALE_SIZE(verticalBoxPads));
    setEo(box);
    expand();
    show();
}

BubbleItemContainer::~BubbleItemContainer()
{
}

void BubbleItemContainer::append(Evas_Object *item)
{
    evas_object_size_hint_align_set(item, 0.0, EVAS_HINT_FILL);
    evas_object_smart_calculate(item);
    evas_object_show(item);
    elm_box_pack_end(getEo(), item);
}

void BubbleItemContainer::go()
{
    elm_box_recalculate(getEo());
}


