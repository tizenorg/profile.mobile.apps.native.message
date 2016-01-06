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

#include "ConvSelectAll.h"
#include "PathUtils.h"
#include "CallbackAssist.h"

using namespace Msg;

namespace
{
    const char *groupName = "elm/layout/select_all_layout";
    const char *textPart = "elm.select_all_text";
    const char *iconPart = "elm.swallow.select_icon";
    const char *clickedSig = "sel_all_layout,clicked";
}

ConvSelectAll::ConvSelectAll(Evas_Object *parent)
    : m_pCheck(nullptr)
{
    setEo(addLayout(parent, SELECT_ALL_EDJ_PATH, groupName));
    Evas_Object *check = createCheck(getEo());
    setContent(check, iconPart);
    setText(msgt("IDS_MSG_MBODY_SELECT_ALL"));
    elm_object_signal_callback_add(getEo(), clickedSig, "", onLayoutClicked, this);
}

ConvSelectAll::~ConvSelectAll()
{

}

void ConvSelectAll::setText(const std::string &text)
{
    View::setText(text, textPart);
}

void ConvSelectAll::setText(const TText &text)
{
    View::setText(text, textPart);
}

void ConvSelectAll::addCheckCallback(Evas_Smart_Cb cb, void *userData)
{
    evas_object_smart_callback_add(m_pCheck, "changed", cb, userData);
}

Evas_Object *ConvSelectAll::createCheck(Evas_Object *parent)
{
    m_pCheck = elm_check_add(parent);
    evas_object_propagate_events_set(m_pCheck, EINA_FALSE);
    evas_object_show(m_pCheck);
    return m_pCheck;
}

bool ConvSelectAll::getCheckState() const
{
    return elm_check_state_get(m_pCheck);
}

void ConvSelectAll::setCheckState(bool check)
{
    elm_check_state_set(m_pCheck, check);
}

void ConvSelectAll::onLayoutClicked(void *data, Evas_Object *obj, const char *emission, const char *source)
{
    Evas_Object *check = static_cast<ConvSelectAll*>(data)->m_pCheck;
    elm_check_state_set(check, !elm_check_state_get(check));
    evas_object_smart_callback_call(check, "changed", nullptr);
}
