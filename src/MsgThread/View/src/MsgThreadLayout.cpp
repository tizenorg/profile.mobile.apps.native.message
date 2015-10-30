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

#include "MsgThreadLayout.h"
#include "ResourceUtils.h"
#include "Logger.h"

using namespace Msg;

MsgThreadLayout::MsgThreadLayout(Evas_Object *parent)
    : DefaultLayout(parent)
{
    create(parent);
}

MsgThreadLayout::~MsgThreadLayout()
{

}

void MsgThreadLayout::create(Evas_Object *parent)
{
    m_pLayout = elm_layout_add(parent);
    evas_object_show(m_pLayout);
    std::string path = ResourceUtils::getResourcePath(MSG_THREAD_EDJ_PATH);
    elm_layout_file_set(m_pLayout, path.c_str(), "msg_thread");
    DefaultLayout::setContent(m_pLayout);
}

void MsgThreadLayout::setList(Evas_Object *list)
{
    elm_object_part_content_set(m_pLayout, "swl.list", list);
}

void MsgThreadLayout::setBg(Evas_Object *bg)
{
    elm_object_part_content_set(m_pLayout, "swl.bg", bg);
}

void MsgThreadLayout::setSearchPanel(Evas_Object *panel)
{
    elm_object_part_content_set(m_pLayout, "swl.searchbar", panel);
}

void MsgThreadLayout::showSearchPanel(bool vale)
{
    const char *sig = vale ? "show_searchbar" : "hide_searchbar";
    elm_object_signal_emit(m_pLayout, sig, "");
}
