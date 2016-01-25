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

#include "ConversationLayout.h"
#include "PathUtils.h"

using namespace Msg;

ConversationLayout::ConversationLayout(Evas_Object *parent)
    : View()
    , m_pLayout(nullptr)
{
    create(parent);
}

ConversationLayout::~ConversationLayout()
{
}

void ConversationLayout::showContactList(bool value)
{
    const char *sig = value ? "show.predictsearch" : "hide.predictsearch";
    elm_object_signal_emit(m_pLayout, sig, "*");
}

void ConversationLayout::showSelectAll(bool value)
{
    const char *sig = value ? "show_select_all" : "hide_select_all";
    elm_object_signal_emit(m_pLayout, sig, "*");
}

void ConversationLayout::setRecipientRect(Evas_Object *layout)
{
    elm_object_part_content_set(m_pLayout, "swl.recipient.rect", layout);
}

void ConversationLayout::setConvList(Evas_Object *layout)
{
    elm_object_part_content_set(m_pLayout, "swl.bubble", layout);
}

void ConversationLayout::setSelectAll(Evas_Object *layout)
{
    elm_object_part_content_set(m_pLayout, "swl.sel_al", layout);
}

void ConversationLayout::setContactList(Evas_Object *layout)
{
    elm_object_part_content_set(m_pLayout, "swl.predictsearch", layout);
}

void ConversationLayout::setMsgInputPanel(Evas_Object *layout)
{
    elm_object_part_content_set(m_pLayout, "swl.msg_input_panel", layout);
}

void ConversationLayout::setRecipientPanel(Evas_Object *layout)
{
    elm_object_part_content_set(m_pLayout, "swl.recipient", layout);
}

void ConversationLayout::create(Evas_Object *parent)
{
    m_pLayout = createMainLayout(parent);
    setEo(m_pLayout);
}

Evas_Object *ConversationLayout::createMainLayout(Evas_Object *parent)
{
    Evas_Object *layout = elm_layout_add(parent);
    std::string edjePath = PathUtils::getResourcePath(CONV_LAYOUT_EDJ_PATH);
    elm_layout_file_set(layout, edjePath.c_str(), "conv_layout");
    evas_object_show(layout);
    return layout;
}

