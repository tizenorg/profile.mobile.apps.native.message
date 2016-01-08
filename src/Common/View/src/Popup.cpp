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

#include "Popup.h"
#include "Logger.h"
#include "PopupManager.h"
#include "Window.h"

#include <sstream>
#include <Elementary.h>

using namespace Msg;

namespace
{
    const int defaultHeight = 300;
    const char *buttonIdKey = "id";
    const char *buttonCbKey = "cb";
    const char *buttonCbDataKey = "cb-data";
    const char *titlePart = "title,text";
}

Popup::Popup(Evas_Object *parent)
    : m_pBox(nullptr)
    , m_pContent(nullptr)
    , m_CurrentButtonIndex(1)
    , m_pManager(nullptr)
{
    create(parent);
}

Popup::Popup(PopupManager &parent)
    : m_pBox(nullptr)
    , m_pContent(nullptr)
    , m_CurrentButtonIndex(1)
    , m_pManager(&parent)
{
    create(parent.getWindow());
}

Popup::~Popup()
{
    MSG_LOG("Destructor");
}

void Popup::create(Evas_Object *parent)
{
    setEo(elm_popup_add(parent));
    elm_popup_orient_set(getEo(), ELM_POPUP_ORIENT_BOTTOM);
    elm_popup_align_set(getEo(), ELM_NOTIFY_ALIGN_FILL, 1.0);
    expand();

    m_pBox = elm_box_add(getEo());
    elm_box_homogeneous_set(m_pBox, EINA_FALSE);
    expand(m_pBox);
    evas_object_size_hint_min_set(m_pBox, 0, defaultHeight);
    evas_object_show(m_pBox);

    elm_object_content_set(getEo(), m_pBox);
}

void Popup::destroy()
{
    if(m_pManager)
        m_pManager->resetPopup();
    else
        View::destroy();
}

Evas_Object *Popup::setContent(Evas_Object *content)
{
    Evas_Object *oldContent = m_pContent;
    elm_box_unpack_all(m_pBox);
    elm_box_pack_start(m_pBox, content);
    m_pContent = content;
    evas_object_show(m_pContent);
    return oldContent;
}

void Popup::setContent(const std::string &text)
{
    setText(text);
}

void Popup::setContent(const TText &text)
{
    setText(text);
}

void Popup::setHeight(int height)
{
    evas_object_size_hint_min_set(m_pBox, 0, height);
}

Evas_Object *Popup::addButton(const TText &text, int buttonId, PopupButtonCb buttonCb, void *userData)
{
    std::stringstream ss;
    ss << "button" << m_CurrentButtonIndex;

    Evas_Object *btn = elm_button_add(getEo());
    View::setContent(btn, ss.str().c_str());

    if(elm_object_part_content_get(getEo(), ss.str().c_str()))
    {
        ++m_CurrentButtonIndex;
        evas_object_show(btn);
        evas_object_data_set(btn, buttonIdKey, (void*)buttonId);
        evas_object_data_set(btn, buttonCbKey, (void*)buttonCb);
        evas_object_data_set(btn, buttonCbDataKey, userData);
        evas_object_smart_callback_add(btn, "clicked", on_button_clicked, this);
        setText(btn, text);
    }
    else
    {
        MSG_LOG_ERROR("Can't set content to part: ", ss.str());
        evas_object_del(btn);
        btn = nullptr;
    }

    return btn;
}

Evas_Object *Popup::getHostEvasObject() const
{
    return m_pBox;
}

Evas_Object *Popup::getContent() const
{
    return m_pContent;
}

void Popup::setTitle(const std::string &title)
{
    setText(title, titlePart);
}

void Popup::setTitle(const TText &title)
{
    setText(title, titlePart);
}

void Popup::on_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
    PopupButtonCb cb = (PopupButtonCb)evas_object_data_get(obj, buttonCbKey);
    if(cb)
    {
        void *userData = evas_object_data_get(obj, buttonCbDataKey);
        Popup *popup = static_cast<Popup*>(data);
        int buttonId = (int)evas_object_data_get(obj, buttonIdKey);
        cb(*popup, buttonId, userData);
    }
}

void Popup::defaultButtonCb(Popup &popup, int buttonId, void *userData)
{
    if(popup.m_pManager)
        popup.m_pManager->resetPopup();
}
