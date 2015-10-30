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

#include "DefaultLayout.h"

using namespace Msg;

DefaultLayout::DefaultLayout(Evas_Object *parent)
{
    setEo(elm_layout_add(parent));
    elm_layout_theme_set(getEo(), "layout", "application", "default");
}

DefaultLayout::~DefaultLayout()
{

}

void DefaultLayout::setContent(Evas_Object *obj)
{
    elm_object_part_content_set(getEo(), "elm.swallow.content", obj);
}

void DefaultLayout::setBg(Evas_Object *obj)
{
    elm_object_part_content_set(getEo(), "elm.swallow.bg", obj);
}

void DefaultLayout::setFloatingButton(Evas_Object *obj)
{
    elm_object_part_content_set(getEo(), "elm.swallow.floatingbutton", obj);
}

