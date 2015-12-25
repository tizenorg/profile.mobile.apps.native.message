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

#include "PageSeparator.h"
#include "BodyView.h"

using namespace Msg;

namespace
{
    const char *pageSeparatorGroup = "conv/body/page_divider";
    const char *itemLayoutGroup = "conv/body/item_lyaout";
}

PageSeparator::PageSeparator(BodyView &parent)
    : BodyViewItem(SeparatorType)
{
    setEo(elm_layout_add(parent));
    elm_layout_file_set(getEo(), getEdjPath().c_str(), itemLayoutGroup);
    emitSignal("show.normal.mode", "*");
    expand();
    show();

    m_pTextLayout = elm_layout_add(getEo());
    evas_object_show(m_pTextLayout);
    elm_layout_file_set(m_pTextLayout, getEdjPath().c_str(), pageSeparatorGroup);

    setContent(m_pTextLayout, "swl.content");
}

PageSeparator::~PageSeparator()
{

}

void PageSeparator::setText(const std::string &text)
{
    elm_object_part_text_set(m_pTextLayout, "text.page_info", text.c_str());
}
