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

#include "PageView.h"
#include "BodyView.h"

#include <efl_extension.h>

using namespace Msg;

namespace
{
    const char *itemLayoutGroup = "conv/body/item_lyaout";
    const char *pageSeparatorGroup = "conv/body/page_divider";
    const char *pageGroup = "conv/body/page";
}

BodyViewItem::BodyViewItem(Evas_Object *parent, Type type)
    : m_Type(type)
{
    setEo(elm_layout_add(parent));
    elm_layout_file_set(getEo(), getEdjPath().c_str(), itemLayoutGroup);
    emitSignal("show.normal.mode", "*");
    expand();
    show();
}

BodyViewItem::~BodyViewItem()
{
}

std::string BodyViewItem::getEdjPath() const
{
    return ResourceUtils::getResourcePath(MSG_BODY_EDJ_PATH);
}

void BodyViewItem::setChild(Evas_Object *item)
{
    elm_object_part_content_set(getEo(), "swl.content", item);
}

BodyViewItem::Type BodyViewItem::getType() const
{
    return m_Type;
}

PageSeparator::PageSeparator(BodyView &parent)
    : BodyViewItem(parent, SeparatorType)
    , m_pLayout(nullptr)
{
    m_pLayout = elm_layout_add(getEo());
    evas_object_show(m_pLayout);
    elm_layout_file_set(m_pLayout, getEdjPath().c_str(), pageSeparatorGroup);
    setChild(m_pLayout);
}

PageSeparator::~PageSeparator()
{

}

void PageSeparator::setText(const std::string &text)
{
    elm_object_part_text_set(m_pLayout, "text.page_info", text.c_str());
}

PageView::PageView(BodyView &parent)
    : BodyViewItem(parent, PageType)
    , m_pEntry(nullptr)
    , m_Body(parent)
{
    Evas_Object *layout = createLayout(getEo());
    Evas_Object *box = createBox(layout);
    Evas_Object *entry = createEntry(box);

    elm_box_pack_end(box, entry);
    elm_object_part_content_set(layout, "swl.page", box);
    setChild(layout);
}

PageView::~PageView()
{

}

Evas_Object *PageView::createLayout(Evas_Object *parent)
{
    Evas_Object *layout = elm_layout_add(parent);
    evas_object_show(layout);
    elm_layout_file_set(layout, getEdjPath().c_str(), pageGroup);
    expand(layout);
    return layout;
}

Evas_Object *PageView::createBox(Evas_Object *parent)
{
    Evas_Object *box = elm_box_add(parent);
    evas_object_show(box);
    return box;
}

void PageView::setGuideText(const TText &text)
{
    setText(m_pEntry, text, "elm.guide");
    int cur = elm_entry_cursor_pos_get(m_pEntry);
    if(!cur)
    {
        elm_entry_cursor_line_end_set(m_pEntry);
    }
}

void PageView::clearText()
{
    elm_object_text_set(m_pEntry, "");
}

std::string PageView::getText() const
{
    return m_pEntry ? elm_object_text_get(m_pEntry) : "";
}

std::string PageView::getPlainUtf8Text() const
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

BodyView &PageView::getBody()
{
    return m_Body;
}

const BodyView &PageView::getBody() const
{
    return m_Body;
}

bool PageView::isEmpty() const
{
    const char *text = elm_entry_entry_get(m_pEntry);
    return text ? text[0] == '\0' : true;
}

Evas_Object *PageView::createEntry(Evas_Object *parent)
{
    m_pEntry= elm_entry_add(parent);
    elm_entry_prediction_allow_set(m_pEntry, EINA_TRUE);
    elm_entry_autocapital_type_set(m_pEntry, ELM_AUTOCAPITAL_TYPE_SENTENCE);
    elm_entry_input_panel_return_key_type_set(m_pEntry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_DEFAULT);
    elm_object_focus_allow_set(m_pEntry, true);
    evas_object_show(m_pEntry);
    eext_entry_selection_back_event_allow_set(m_pEntry, EINA_TRUE);
    expand(m_pEntry);

    evas_object_smart_callback_add(m_pEntry, "changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onChanged(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "cursor,changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onCursorChanged(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "focused", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onFocused(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "unfocused", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onUnfocused(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onChanged(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "preedit,changed", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onPreeditChanged(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "press", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onPress(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "clicked", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onClicked(*page);
    }, this);

    evas_object_smart_callback_add(m_pEntry, "maxlength,reached", [](void *data, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onMaxLengthReached(*page);
    }, this);

    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_DOWN, [](void *data, Evas *e, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onKeyDown(*page);
    }, this);

    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_UP, [](void *data, Evas *e, Evas_Object *obj, void *event_info)
    {
        PageView *page = static_cast<PageView*>(data);
        static_cast<PageView*>(data)->m_Body.onKeyUp(*page);
    }, this);

    return m_pEntry;
}

