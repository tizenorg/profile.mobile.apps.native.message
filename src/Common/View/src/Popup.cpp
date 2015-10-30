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

#include <sstream>
#include <Elementary.h>

using namespace Msg;

namespace
{
    const int defaultHeight = 300;
    const char *buttonIdKey = "id";
    const char *buttonCbKey = "cb";
    const char *buttonCbDataKey = "cb-data";
}

Popup::Popup(Evas_Object *parent)
    : View()
    , m_pBox(nullptr)
    , m_pContent(nullptr)
    , m_CurrentButtonIndex(1)
    , m_pListener(nullptr)
    , m_Type(0)
{
    create(parent);
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
    evas_object_size_hint_weight_set(m_pBox, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_pBox, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_min_set(m_pBox, 0, defaultHeight);
    evas_object_show(m_pBox);

    elm_object_content_set(getEo(), m_pBox);
}

void Popup::setUserType(int type)
{
    m_Type = type;
}

int Popup::getUserType() const
{
    return m_Type;
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
    elm_object_text_set(getEo(), text.c_str());
}

void Popup::setHeight(int height)
{
    evas_object_size_hint_min_set(m_pBox, 0, height);
}

Evas_Object *Popup::addButton(const std::string &text, int buttonId, PopupCb popupCb, void *userData)
{
    std::stringstream ss;
    ss << "button" << m_CurrentButtonIndex;

    Evas_Object *btn = elm_button_add(getEo());
    elm_object_part_content_set(getEo(), ss.str().c_str(), btn);

    if(elm_object_part_content_get(getEo(), ss.str().c_str()))
    {
        ++m_CurrentButtonIndex;
        evas_object_show(btn);
        evas_object_data_set(btn, buttonIdKey, (void*)buttonId);
        evas_object_data_set(btn, buttonCbKey, (void*)popupCb);
        evas_object_data_set(btn, buttonCbDataKey, userData);
        evas_object_smart_callback_add(btn, "clicked", on_button_clicked, this);
        elm_object_text_set(btn, text.c_str());
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

void Popup::onHwBackButtonClicked()
{

}

void Popup::setListener(IPopupListener *listener)
{
    m_pListener = listener;
}

void Popup::setTitle(const std::string &title)
{
    elm_object_part_text_set(getEo(), "title,text", title.c_str());
}

void Popup::onHwMoreButtonClicked()
{

}

void Popup::on_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
    Popup *popup = static_cast<Popup*>(data);

    if(popup && popup->m_pListener)
    {
        int buttonId = (int)evas_object_data_get(obj, buttonIdKey);
        PopupCb cb = (PopupCb)evas_object_data_get(obj, buttonCbKey);

        if(cb)
        {
            void *userData = evas_object_data_get(obj, buttonCbDataKey);
            cb(*popup, buttonId, userData);
        }
        else if(popup->m_pListener)
        {
            popup->m_pListener->onPopupButtonClicked(*popup, buttonId);
        }

    }
}

