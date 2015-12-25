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

#include "ConvListLayout.h"

using namespace Msg;

namespace
{
    const char *layoutStyle = "conv_list_base";
}

ConvListLayout::ConvListLayout(Evas_Object *parent)
{
    create(parent);
}

ConvListLayout::~ConvListLayout()
{

}

void ConvListLayout::showSelectAllMode(bool value)
{
    const char *signal = value ? "show.select":"hide.select";
    emitSignal(signal, "*");
}

void ConvListLayout::setBubbleList(Evas_Object *layout)
{
    elm_object_part_content_set(getEo(), "swl.list", layout);
}

void ConvListLayout::setSelectAll(Evas_Object *layout)
{
    elm_object_part_content_set(getEo(), "swl.select", layout);
}

void ConvListLayout::create(Evas_Object *parent)
{
    setEo(elm_layout_add(parent));
    elm_layout_file_set(getEo(), ResourceUtils::getResourcePath(CONV_LIST_EDJ_PATH).c_str(), layoutStyle);
    expand();
    show();
}
