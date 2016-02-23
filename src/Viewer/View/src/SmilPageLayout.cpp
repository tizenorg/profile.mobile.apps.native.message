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

#include "SmilPageLayout.h"

using namespace Msg;

SmilPageLayout::SmilPageLayout(Evas_Object *parent)
    : m_pBox(nullptr)
{
    setEo(createScroller(parent));
    Evas_Object *box = createBox(getEo());
    setContent(box);
}

SmilPageLayout::~SmilPageLayout()
{

}

Evas_Object *SmilPageLayout::getBox() const
{
    return m_pBox;
}

void SmilPageLayout::appendItem(Evas_Object *item)
{
    expand(item);
    evas_object_show(item);
    elm_box_pack_end(m_pBox, item);
}

Evas_Object *SmilPageLayout::createScroller(Evas_Object *parent)
{
    Evas_Object *scroller = elm_scroller_add(parent);
    elm_object_style_set(scroller, "effect");
    elm_scroller_bounce_set(scroller, false, true);
    elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
    return scroller;
}

Evas_Object *SmilPageLayout::createBox(Evas_Object *parent)
{
    m_pBox = elm_box_add(parent);
    evas_object_size_hint_weight_set(m_pBox, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(m_pBox, EVAS_HINT_FILL, 0.0);
    elm_box_horizontal_set(m_pBox, false);
    elm_box_homogeneous_set(m_pBox, EINA_FALSE);
    evas_object_show(m_pBox);
    return m_pBox;
}
