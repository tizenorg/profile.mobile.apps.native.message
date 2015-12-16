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

#include "BodyViewItem.h"

#include <assert.h>

using namespace Msg;

namespace
{
    const char *itemLayoutGroup = "conv/body/item_lyaout";
}

BodyViewItem::BodyViewItem(Evas_Object *parent, Type type)
    : m_Type(type)
{
    setEo(elm_layout_add(parent));
    elm_layout_file_set(getEo(), getEdjPath().c_str(), itemLayoutGroup);
    emitSignal("show.normal.mode", "*");
    expand();
    show();
}

BodyViewItem::~BodyViewItem()
{
}

std::string BodyViewItem::getEdjPath() const
{
    return ResourceUtils::getResourcePath(MSG_BODY_EDJ_PATH);
}

void BodyViewItem::setChild(Evas_Object *item)
{
    elm_object_part_content_set(getEo(), "swl.content", item);
}

BodyViewItem::Type BodyViewItem::getType() const
{
    return m_Type;
}
