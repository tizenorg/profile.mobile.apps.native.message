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

#include "FloatingButton.h"
#include "CallbackAssist.h"
#include <efl_extension.h>

using namespace Msg;

#define COMPOSE_BUTTON_ICON_SIZE ELM_SCALE_SIZE(80)

FloatingButton::FloatingButton(Evas_Object *parent)
    : m_pListener(nullptr)
{
    setEo(eext_floatingbutton_add(parent));
    Evas_Object *composeBtn = elm_button_add(getEo());

    Evas_Object *icon = elm_image_add(getEo());
    std::string resPath = ResourceUtils::getResourcePath(MSG_THREAD_FLOATING_BTN_ICON);
    elm_image_file_set(icon, resPath.c_str(), NULL);
    evas_object_size_hint_min_set(icon, COMPOSE_BUTTON_ICON_SIZE, COMPOSE_BUTTON_ICON_SIZE);
    elm_object_part_content_set(composeBtn, "icon", icon);

    setContent(composeBtn, "button1");
    evas_object_smart_callback_add(composeBtn, "clicked", SMART_CALLBACK(FloatingButton, pressed), this);

    show();
}

void FloatingButton::setListener(IFloatingButtonListener* pListener)
{
    m_pListener = pListener;
}

void FloatingButton::pressed(Evas_Object *obj, void *event_info)
{
    if(m_pListener)
    {
        m_pListener->onFloatingButtonPressed();
    }
}
