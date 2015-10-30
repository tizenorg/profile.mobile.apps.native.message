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

#include "SettingsListViewItem.h"
#include "ListView.h"
#include "Logger.h"
#include "CallbackAssist.h"

#include <stdlib.h>

using namespace Msg;

ListItemStyleRef SettingsListViewItem::multiLineIconStyle = ListItemStyle::create("multiline_sub.main.1icon");
ListItemStyleRef SettingsListViewItem::oneLineIconStyle = ListItemStyle::create("1line");
ListItemStyleRef SettingsListViewItem::multiLineStyle = ListItemStyle::create("multiline_sub.main");

SettingsListViewItem::SettingsListViewItem(const ListItemStyleRef &style)
    : ListItem(style, ELM_GENLIST_ITEM_NONE, 0)
    , m_ShowCheckButton(false)
    , m_CheckButtonState(false)
    , m_DisabledCheckButton(false)
    , m_pListener(nullptr)
{
    m_ShowCheckButton = style == multiLineIconStyle || style == oneLineIconStyle;
}

SettingsListViewItem::~SettingsListViewItem()
{
}

void SettingsListViewItem::showCheckButton(bool show)
{
    m_ShowCheckButton = show;
}

void SettingsListViewItem::disabledCheckButton(bool disabled)
{
    m_DisabledCheckButton = disabled;
}

void SettingsListViewItem::setCheckButtonState(bool state)
{
    m_CheckButtonState = state;
    update();
}

void SettingsListViewItem::changeCheckButtonState()
{
    setCheckButtonState(!m_CheckButtonState);
}

bool SettingsListViewItem::getCheckButtonState() const
{
    return m_CheckButtonState;
}

void SettingsListViewItem::setMainText(const std::string &text)
{
    m_MainText = text;
}

void SettingsListViewItem::setSubText(const std::string &text)
{
    m_SubText = text;
}

std::string SettingsListViewItem::getListItemText(ListItem &item, const char *part)
{
    if(getStyle().get() == oneLineIconStyle.get())
    {
        if(strcmp(part, "elm.text.main.left") == 0)
            return m_MainText;
    }
    else if(getStyle().get() == multiLineIconStyle.get())
    {
        if(strcmp(part, "elm.text.main") == 0)
            return m_MainText;
        else if(strcmp(part, "elm.text.multiline") == 0)
            return m_SubText;
    }
    else if(getStyle().get() == multiLineStyle.get())
    {
        if(strcmp(part, "elm.text.main") == 0)
            return m_MainText;
        else if(strcmp(part, "elm.text.multiline") == 0)
            return m_SubText;
    }
    else
    {
        MSG_LOG_ERROR("Unknown style");
    }

    return "";
}

Evas_Object *SettingsListViewItem::getListItemContent(ListItem &item, const char *part)
{
    if(getStyle().get() == oneLineIconStyle.get())
    {
        if(strcmp(part, "elm.icon.right") == 0)
            return createCheckButton(*getOwner());
    }
    else if(getStyle().get() == multiLineIconStyle.get())
    {
        return createCheckButton(*getOwner());
    }
    else if(getStyle().get() == multiLineStyle.get())
    {
    }
    else
    {
        MSG_LOG_ERROR("Unknown style");
    }

    return nullptr;
}

Evas_Object *SettingsListViewItem::createCheckButton(Evas_Object *parent)
{
    Evas_Object *check = nullptr;
    if(m_ShowCheckButton)
    {
        check = elm_check_add(parent);
        elm_object_focus_allow_set(check, false);
        elm_object_style_set(check, "on&off");
        elm_object_disabled_set(check, m_DisabledCheckButton);
        elm_check_state_set(check, m_CheckButtonState);
        elm_check_state_pointer_set(check, &m_CheckButtonState);
        evas_object_propagate_events_set(check, EINA_FALSE);
        evas_object_smart_callback_add(check, "changed", SMART_CALLBACK(SettingsListViewItem, onCheckButtonChanged), this);
    }
    return check;
}

void SettingsListViewItem::setListener(ISettingsListViewItemListener *listener)
{
    m_pListener = listener;
}

void SettingsListViewItem::onCheckButtonChanged(Evas_Object *obj, void *eventInfo)
{
    if(m_pListener)
        m_pListener->onCheckButtonChanged(*this);
}
