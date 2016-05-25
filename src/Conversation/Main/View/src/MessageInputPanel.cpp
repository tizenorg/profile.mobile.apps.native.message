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

#include "MessageInputPanel.h"
#include "PathUtils.h"
#include "CallbackAssist.h"
#include "Logger.h"

namespace
{
    const char *buttonIdKey = "id";
}

using namespace Msg;

MessageInputPanel::MessageInputPanel(Evas_Object *parent)
    : m_pListener(nullptr)
    , m_pLayout(nullptr)
    , m_pSendButton(nullptr)
    , m_pAddButton(nullptr)
{
    create(parent);
}

MessageInputPanel::~MessageInputPanel()
{

}

void MessageInputPanel::create(Evas_Object *parent)
{
    m_pLayout = elm_layout_add(parent);
    setEo(m_pLayout);
    show();

    std::string edjPath = PathUtils::getResourcePath(MSG_INPUT_PANEL_EDJ_PATH);
    elm_layout_file_set(m_pLayout, edjPath.c_str(), "conversation/msg_input_panel");

    m_pSendButton = createButton(m_pLayout, CONV_SEND_IMG, "send_custom", SendButtonId);
    m_pAddButton = createButton(m_pLayout, CONV_ADD_IMG, "body_button", AddButtonId);

    elm_object_part_content_set(m_pLayout, "swl.send_button", m_pSendButton);
    elm_object_part_content_set(m_pLayout, "swl.add_button", m_pAddButton);
}

void MessageInputPanel::setEntry(Evas_Object *obj)
{
    elm_object_part_content_set(m_pLayout, "swl.main", obj);
}

Evas_Object *MessageInputPanel::getIcon(Evas_Object *button)
{
    return elm_object_part_content_get(button, "button_center_part");
}

void MessageInputPanel::setNormalColor(Evas_Object *button)
{
    evas_object_color_set(getIcon(button), BUTTON_COLOR);
}

void MessageInputPanel::setPressedColor(Evas_Object *button)
{
    evas_object_color_set(getIcon(button), BUTTON_PRESSED_COLOR);
}

void MessageInputPanel::setDisabledColor(Evas_Object *button)
{
    evas_object_color_set(getIcon(button), BUTTON_DISABLED_COLOR);
}

Evas_Object *MessageInputPanel::getButton(ButtonId id)
{
    switch(id)
    {
        case AddButtonId:
            return m_pAddButton;
            break;

        case SendButtonId:
            return m_pSendButton;
            break;

        default:
            MSG_ASSERT(false, "Unknown button id");
            break;
    }

    return nullptr;
}

Evas_Object *MessageInputPanel::createButton(Evas_Object *parent, const char *iconRes, const char *style, ButtonId buttonId)
{
    Evas_Object *button = elm_button_add(parent);
    elm_object_style_set(button, style);
    evas_object_data_set(button, buttonIdKey, (void*)buttonId);
    evas_object_size_hint_weight_set(button, 0.0, 0.0);
    evas_object_size_hint_align_set(button, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(button);

    Evas_Object *icon = elm_icon_add(button);
    std::string resPath = PathUtils::getResourcePath(IMAGES_EDJ_PATH);
    evas_object_show(icon);
    elm_image_file_set(icon, resPath.c_str(), iconRes);
    evas_object_color_set(icon, BUTTON_COLOR);

    elm_object_part_content_set(button, "button_center_part", icon);

    evas_object_smart_callback_add(button, "clicked", SMART_CALLBACK(MessageInputPanel, onButtonClicked), this);
    evas_object_smart_callback_add(button, "pressed", SMART_CALLBACK(MessageInputPanel, onButtonPressed), this);
    evas_object_smart_callback_add(button, "unpressed", SMART_CALLBACK(MessageInputPanel, onButtonUnpressed), this);

    return button;
}

void MessageInputPanel::disabledButton(ButtonId id, bool val)
{
    Evas_Object *btn = getButton(id);
    elm_object_disabled_set(btn, val);

    if(val)
        setDisabledColor(btn);
    else
        setNormalColor(btn);
}

void MessageInputPanel::setCounter(const std::string &text)
{
    elm_object_part_text_set(m_pLayout, "text.char_count", text.c_str());
}

void MessageInputPanel::setListener(IMessageInputPanelListener *l)
{
    m_pListener = l;
}

void MessageInputPanel::onButtonClicked(Evas_Object *obj, void *event_info)
{
    if(m_pListener)
    {
        int id = (int)(intptr_t)evas_object_data_get(obj, buttonIdKey);
        m_pListener->onButtonClicked(*this, (ButtonId)id);
    }
}

void MessageInputPanel::onButtonPressed(Evas_Object *obj, void *event_info)
{
    setPressedColor(obj);
}

void MessageInputPanel::onButtonUnpressed(Evas_Object *obj, void *event_info)
{
    setNormalColor(obj);
}
