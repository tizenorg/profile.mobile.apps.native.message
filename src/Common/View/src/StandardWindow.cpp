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

#include "Config.h"
#include "StandardWindow.h"

#include <Elementary.h>

using namespace Msg;

StandardWindow::StandardWindow()
    : Window()
    , m_pConform(nullptr)
    , m_pBg(nullptr)
{
    // Window:
    setEo(elm_win_add(nullptr, PACKAGE_NAME, ELM_WIN_NOTIFICATION));
    elm_win_conformant_set(getEo(), EINA_TRUE);
    elm_win_autodel_set(getEo(), EINA_TRUE);

    // Conform:
    m_pConform = elm_conformant_add(getEo());
    elm_win_indicator_mode_set(getEo(), ELM_WIN_INDICATOR_SHOW);
    elm_win_indicator_opacity_set(getEo(), ELM_WIN_INDICATOR_OPAQUE);
    evas_object_size_hint_weight_set(m_pConform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(getEo(), m_pConform);
    evas_object_show(m_pConform);

    // Bg:
    m_pBg = elm_bg_add(m_pConform);
    evas_object_size_hint_weight_set(m_pBg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(m_pBg);

    elm_object_part_content_set(m_pConform, "elm.swallow.bg", m_pBg);
}

StandardWindow::~StandardWindow()
{
    evas_object_del(getEo());
}

void StandardWindow::lower()
{
    elm_win_lower(getEo());
}

void StandardWindow::raise()
{
    elm_win_raise(getEo());
}

Evas_Object *StandardWindow::getWinEvasObject() const
{
    return getEo();
}

Evas_Object *StandardWindow::getConformEvasObject() const
{
    return m_pConform;
}

Evas_Object *StandardWindow::getHostEvasObject() const
{
    return m_pConform;
}

void StandardWindow::setContent(Evas_Object *content)
{
    elm_object_content_set(m_pConform, content);
}

void StandardWindow::getScreenSize(int *x, int *y, int *w, int *h) const
{
    elm_win_screen_size_get(getEo(), x, y, w, h);
}

int StandardWindow::getRotation() const
{
    return elm_win_rotation_get(getEo());
}

