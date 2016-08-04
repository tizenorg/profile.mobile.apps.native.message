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

#include "MsgThreadSearchPanel.h"
#include "CallbackAssist.h"
#include "PathUtils.h"

#include <efl_extension.h>

using namespace Msg;

MsgThreadSearchPanel::MsgThreadSearchPanel(Evas_Object *parent)
    : m_pEntry(nullptr)
    , m_pClearButton(nullptr)
    , m_pListener(nullptr)
    , m_EntryFocus(false)
    , m_pEntryJob(nullptr)
{
    create(parent);
}

MsgThreadSearchPanel::~MsgThreadSearchPanel()
{
    if(m_pEntryJob)
    {
        ecore_job_del(m_pEntryJob);
        m_pEntryJob = nullptr;
    }
}

void MsgThreadSearchPanel::create(Evas_Object *parent)
{
    setEo(elm_layout_add(parent));
    elm_layout_theme_set(getEo(), "layout", "searchfield", "singleline");
    expand();

    Evas_Object *entry = createEntry(getEo());
    setContent(entry, "elm.swallow.content");
}

bool MsgThreadSearchPanel::isEmpty() const
{
    const char *text = elm_entry_entry_get(m_pEntry);
    return text ? *text == 0 : true;
}

void MsgThreadSearchPanel::setListener(IMsgThreadSearchPanelListener *listener)
{
    m_pListener = listener;
}

Evas_Object *MsgThreadSearchPanel::createClearButton(Evas_Object *parent)
{
    m_pClearButton = createButton(parent, "editfield_clear");
    return m_pClearButton;
}

Evas_Object *MsgThreadSearchPanel::createEntry(Evas_Object *parent)
{
    m_pEntry = elm_entry_add(parent);
    evas_object_size_hint_weight_set(m_pEntry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_pEntry, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_entry_editable_set(m_pEntry, EINA_TRUE);
    elm_entry_single_line_set(m_pEntry, EINA_TRUE);
    elm_entry_scrollable_set(m_pEntry, EINA_TRUE);
    eext_entry_selection_back_event_allow_set(m_pEntry, EINA_TRUE);
    elm_scroller_policy_set(m_pEntry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
    elm_entry_autocapital_type_set(m_pEntry, ELM_AUTOCAPITAL_TYPE_NONE);
    elm_entry_input_panel_return_key_type_set(m_pEntry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_SEARCH);
    elm_object_focus_allow_set(m_pEntry, EINA_TRUE);
    evas_object_show(m_pEntry);

    evas_object_smart_callback_add(m_pEntry, "changed", SMART_CALLBACK(MsgThreadSearchPanel, onEntryChanged), this);;
    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_DOWN, EVAS_EVENT_CALLBACK(MsgThreadSearchPanel, onKeyDown), this);

    Evas_Object *btn = createClearButton(m_pEntry);
    evas_object_show(btn);
    elm_object_focus_allow_set(btn, false);
    evas_object_smart_callback_add(btn, "clicked", SMART_CALLBACK(MsgThreadSearchPanel, onClearButtonClicked), this);;
    elm_object_part_content_set(getEo(), "elm.swallow.button", btn);

    return m_pEntry;
}

Evas_Object *MsgThreadSearchPanel::createButton(Evas_Object *parent, const char *style)
{
    Evas_Object *button = elm_button_add(parent);
    elm_object_style_set(button, style);
    evas_object_show(button);
    return button;
}

std::string MsgThreadSearchPanel::getEntryText() const
{
    std::string res;
    const char *plainText = elm_entry_entry_get(m_pEntry);

    if(plainText)
    {
        char *text = elm_entry_markup_to_utf8(elm_entry_entry_get(m_pEntry));
        if(text)
        {
            res.assign(text);
            free(text);
        }
    }
    return res;
}

void MsgThreadSearchPanel::onPostponedEntryFocus(void *data)
{
    MsgThreadSearchPanel *self = static_cast<MsgThreadSearchPanel*>(data);
    self->m_pEntryJob = nullptr;
    elm_object_focus_set(self->m_pEntry, self->m_EntryFocus);
}

void MsgThreadSearchPanel::setEntryFocus(bool focus)
{
    m_EntryFocus = focus;
    if(!m_pEntryJob)
        m_pEntryJob = ecore_job_add(onPostponedEntryFocus, this);
}

void MsgThreadSearchPanel::clearEntry()
{
    elm_entry_entry_set(m_pEntry, "");
}

void MsgThreadSearchPanel::showClearButton(bool visible)
{
    const char *sig = visible ? "elm,action,show,button" : "elm,action,hide,button";
    emitSignal(sig, "");
}

void MsgThreadSearchPanel::setGuideText(const std::string &text)
{
    elm_object_part_text_set(m_pEntry, "elm.guide", text.c_str());
}

void MsgThreadSearchPanel::onEntryChanged(Evas_Object *obj, void *eventInfo)
{
    showClearButton(!isEmpty());
    if(m_pListener)
        m_pListener->onEntryChanged(*this);
}

void MsgThreadSearchPanel::onClearButtonClicked(Evas_Object *obj, void *eventInfo)
{
    clearEntry();
}

void MsgThreadSearchPanel::onKeyDown(Evas_Object *obj, void *eventInfo)
{

}
