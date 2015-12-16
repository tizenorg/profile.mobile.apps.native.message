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

#include "TextPageViewItem.h"
#include "PageView.h"

#include <efl_extension.h>

using namespace Msg;


TextPageViewItem::TextPageViewItem(PageView &parent)
    : PageViewItem(parent)
    , m_pEntry(nullptr)
    , m_pListener(nullptr)
{
    setEo(createEntry(parent.getItemParent()));
}

TextPageViewItem::~TextPageViewItem()
{

}

TextPageViewItem::Type TextPageViewItem::getType() const
{
    return TextType;
}

bool TextPageViewItem::isEmpty() const
{
    const char *text = elm_entry_entry_get(m_pEntry);
    return text ? text[0] == '\0' : true;
}

void TextPageViewItem::setListener(ITextPageViewItemListener *l)
{
    m_pListener = l;
}

void TextPageViewItem::notifyListener(void *data, ListenerMethod method)
{
    TextPageViewItem *self = static_cast<TextPageViewItem*>(data);
    if(self && self->m_pListener)
    {
        (self->m_pListener->*method)(*self);
    }
}

void TextPageViewItem::setGuideText(const TText &text)
{
    setText(m_pEntry, text, "elm.guide");
    int cur = elm_entry_cursor_pos_get(m_pEntry);
    if(!cur)
    {
        elm_entry_cursor_line_end_set(m_pEntry);
    }
}

void TextPageViewItem::clearText()
{
    elm_object_text_set(m_pEntry, "");
}

std::string TextPageViewItem::getText() const
{
    return m_pEntry ? elm_object_text_get(m_pEntry) : "";
}

std::string TextPageViewItem::getPlainUtf8Text() const
{
    std::string res;
    const char *plainText = elm_entry_entry_get(m_pEntry);

    if(plainText)
    {
        char *text = elm_entry_markup_to_utf8(plainText);
        if(text)
        {
            res.assign(text);
            free(text);
        }
    }
    return res;
}

Evas_Object *TextPageViewItem::createEntry(Evas_Object *parent)
{
    m_pEntry = elm_entry_add(parent);
    elm_entry_prediction_allow_set(m_pEntry, EINA_TRUE);
    elm_entry_autocapital_type_set(m_pEntry, ELM_AUTOCAPITAL_TYPE_SENTENCE);
    elm_entry_input_panel_return_key_type_set(m_pEntry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT);
    elm_object_focus_allow_set(m_pEntry, true);
    evas_object_show(m_pEntry);
    eext_entry_selection_back_event_allow_set(m_pEntry, EINA_TRUE);
    expand(m_pEntry);

    evas_object_smart_callback_add(m_pEntry, "changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onChanged);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "cursor,changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onCursorChanged);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "focused", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onFocused);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "unfocused", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onUnfocused);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onChanged);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "preedit,changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onPreeditChanged);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "press", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onPress);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "clicked", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onClicked);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "maxlength,reached", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onMaxLengthReached);
    }, this);

    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_DOWN, [](void *data, Evas *e, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onKeyDown);
    }, this);

    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_UP, [](void *data, Evas *e, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &ITextPageViewItemListener::onKeyUp);
    }, this);

    return m_pEntry;
}
