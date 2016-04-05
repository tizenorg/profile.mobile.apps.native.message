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

#include "ConvRecipientsPanelView.h"
#include "CallbackAssist.h"
#include "PathUtils.h"
#include "Logger.h"
#include "LangUtils.h"

#include <efl_extension.h>

using namespace Msg;


ConvRecipientsPanelView::ConvRecipientsPanelView(Evas_Object *parent, int entryMaxCharCount)
    : View()
    , m_pLayout(nullptr)
    , m_pEntry(nullptr)
    , m_pContactBtn(nullptr)
    , m_pRect(nullptr)
    , m_EntryMaxCharCount(entryMaxCharCount)
    , m_IsMbeVisible(false)
    , m_pMbe(nullptr)
{
    create(parent);
}

void ConvRecipientsPanelView::setMbe(MbeRecipientsView *pMbe)
{
    m_pMbe = pMbe;
    addGeometryChangedCb(*m_pMbe);
    elm_object_part_content_set(m_pLayout, "swl.mbe", *m_pMbe);
}

ConvRecipientsPanelView::~ConvRecipientsPanelView()
{
}

void ConvRecipientsPanelView::addGeometryChangedCb(Evas_Object *obj)
{
    evas_object_event_callback_add(obj, EVAS_CALLBACK_RESIZE, EVAS_EVENT_CALLBACK(ConvRecipientsPanelView, onGeometryChanged), this);
    evas_object_event_callback_add(obj, EVAS_CALLBACK_MOVE, EVAS_EVENT_CALLBACK(ConvRecipientsPanelView, onGeometryChanged), this);
    evas_object_event_callback_add(obj, EVAS_CALLBACK_SHOW, EVAS_EVENT_CALLBACK(ConvRecipientsPanelView, onGeometryChanged), this);
    evas_object_event_callback_add(obj, EVAS_CALLBACK_HIDE, EVAS_EVENT_CALLBACK(ConvRecipientsPanelView, onGeometryChanged), this);
    evas_object_event_callback_add(obj, EVAS_CALLBACK_CHANGED_SIZE_HINTS, EVAS_EVENT_CALLBACK(ConvRecipientsPanelView, onGeometryChanged), this);
}

bool ConvRecipientsPanelView::getEntryFocus() const
{
    return elm_object_focus_get(m_pEntry);
}

void ConvRecipientsPanelView::setEntryFocus(bool val)
{
    elm_object_focus_set(m_pEntry, val);
}

void ConvRecipientsPanelView::clearEntry()
{
    elm_object_text_set(m_pEntry, "");
}

void ConvRecipientsPanelView::clearMbe()
{
    elm_multibuttonentry_clear(*m_pMbe);
}

void ConvRecipientsPanelView::clear()
{
    clearEntry();
    clearMbe();
}

void ConvRecipientsPanelView::showMbe(bool show)
{
    m_IsMbeVisible = show;
    const char *sig = show ? "show_to_mbe" : "hide_to_mbe";
    elm_object_signal_emit(m_pLayout, sig, "*");
}

void ConvRecipientsPanelView::showEntry(bool show)
{
    const char *sig = show ? "show_entry" : "hide_entry";
    elm_object_signal_emit(m_pLayout, sig, "*");
}

bool ConvRecipientsPanelView::isMbeEmpty() const
{
    return elm_multibuttonentry_first_item_get(*m_pMbe) == nullptr;
}

bool ConvRecipientsPanelView::isMbeVisible() const
{
    return m_IsMbeVisible;
}

std::string ConvRecipientsPanelView::getEntryText() const
{
    std::string result;

    const char *text = elm_entry_entry_get(m_pEntry);

    if(text)
    {
        char *utf8 = elm_entry_markup_to_utf8(text);

        if(utf8)
            result.assign(utf8);
        free(utf8);
    }

    return result;
}

void ConvRecipientsPanelView::setEntryText(const std::string &utf8)
{
    char *text = elm_entry_utf8_to_markup(utf8.c_str());

    if(text)
    {
        elm_entry_entry_set(m_pEntry, text);
        elm_entry_cursor_end_set(m_pEntry);
        free(text);
    }
}

void ConvRecipientsPanelView::create(Evas_Object *parent)
{
    Evas_Object *box = elm_box_add(parent);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
    setEo(box);
    show();

    m_pLayout = elm_layout_add(box);
    evas_object_size_hint_weight_set(m_pLayout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_pLayout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(m_pLayout);

    elm_box_pack_end(box, m_pLayout);

    std::string path = PathUtils::getResourcePath(RECIPIENT_PANEL_EDJ_PATH);
    elm_layout_file_set(m_pLayout, path.c_str(), "recipient_panel");

    createAreaRect(m_pLayout);
    Evas_Object *entry =  createEntry(m_pLayout);
    Evas_Object *button = createContactBtn(m_pLayout);

    elm_object_part_content_set(m_pLayout, "swl.entry", entry);
    elm_object_part_content_set(m_pLayout, "swl.contact_btn", button);
}

Evas_Object *ConvRecipientsPanelView::createEntry(Evas_Object *parent)
{
    m_pEntry = elm_entry_add(parent);
    evas_object_size_hint_weight_set(m_pEntry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_pEntry, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_signal_emit(m_pEntry, "elm,action,hide,search_icon", "");
    elm_entry_editable_set(m_pEntry, EINA_TRUE);
    elm_entry_single_line_set(m_pEntry, EINA_TRUE);
    elm_entry_scrollable_set(m_pEntry, EINA_TRUE);
    eext_entry_selection_back_event_allow_set(m_pEntry, EINA_TRUE);
    elm_scroller_policy_set(m_pEntry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
    elm_entry_autocapital_type_set(m_pEntry, ELM_AUTOCAPITAL_TYPE_NONE);
    elm_entry_input_panel_return_key_type_set(m_pEntry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_NEXT);
    setText(m_pEntry, msgt("IDS_MSGF_BODY_RECIPIENTS"), "elm.guide");

    Elm_Entry_Filter_Limit_Size limitFilter = {};
    limitFilter.max_char_count = m_EntryMaxCharCount;
    elm_entry_markup_filter_append(m_pEntry, elm_entry_filter_limit_size, &limitFilter);

    evas_object_smart_callback_add(m_pEntry, "changed", SMART_CALLBACK(ConvRecipientsPanelView, onEntryChanged), this);
    evas_object_smart_callback_add(m_pEntry, "focused", SMART_CALLBACK(ConvRecipientsPanelView, onEntryFocusChanged), this);
    evas_object_smart_callback_add(m_pEntry, "unfocused", SMART_CALLBACK(ConvRecipientsPanelView, onEntryFocusChanged), this);
    evas_object_smart_callback_add(m_pEntry, "clicked", SMART_CALLBACK(ConvRecipientsPanelView, onEntryClicked), this);
    evas_object_smart_callback_add(m_pEntry, "maxlength,reached", SMART_CALLBACK(ConvRecipientsPanelView, onEntryMaxlengthReached), this);
    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_DOWN, EVAS_EVENT_CALLBACK(ConvRecipientsPanelView, onKeyDown), this);
    addGeometryChangedCb(m_pEntry);

    return m_pEntry;
}

Evas_Object *ConvRecipientsPanelView::createAreaRect(Evas_Object *parent)
{
    m_pRect = evas_object_rectangle_add(evas_object_evas_get(parent));
    evas_object_show(m_pRect);
    evas_object_color_set(m_pRect, 0, 0, 0, 0);
    return m_pRect;
}

Evas_Object *ConvRecipientsPanelView::createContactBtn(Evas_Object *parent)
{
    m_pContactBtn = elm_button_add(parent);
    elm_object_style_set(m_pContactBtn, "contact_button");

    Evas_Object *icon = elm_image_add(parent);
    std::string imgPath = PathUtils::getResourcePath(IMAGES_EDJ_PATH);
    elm_image_file_set(icon, imgPath.c_str(), CONTACT_IMG);

    elm_object_part_content_set(m_pContactBtn, "button_center_part", icon);
    elm_image_resizable_set(icon, EINA_TRUE, EINA_TRUE);
    evas_object_color_set(icon, BUTTON_COLOR);

    evas_object_smart_callback_add(m_pContactBtn, "pressed", SMART_CALLBACK(ConvRecipientsPanelView, onContactBtnPressed), this);
    evas_object_smart_callback_add(m_pContactBtn, "unpressed", SMART_CALLBACK(ConvRecipientsPanelView, onContactBtnUnpressed), this);
    evas_object_smart_callback_add(m_pContactBtn, "clicked", SMART_CALLBACK(ConvRecipientsPanelView, onContactBtnClicked), this);

    setContactBtnColor(BUTTON_COLOR);

    return m_pContactBtn;
}

Evas_Object *ConvRecipientsPanelView::getAreaRect() const
{
    return m_pRect;
}

void ConvRecipientsPanelView::onGeometryChanged(Evas_Object *obj, void *event_info)
{
    int y = 0;
    int w = 0;
    int h = 0;
    int lyayoutTop = 0;
    Evas_Object * targetObj =  evas_object_visible_get(m_pEntry) ? m_pEntry : m_pMbe->getEo();

    evas_object_geometry_get(targetObj, nullptr, &y, nullptr, &h);
    int bottom = y + h;

    evas_object_geometry_get(m_pLayout, nullptr, &y, nullptr, nullptr);
    lyayoutTop = y;

    evas_object_size_hint_min_get(m_pRect, &w, nullptr);
    evas_object_size_hint_min_set(m_pRect, w, bottom - lyayoutTop);
}

void ConvRecipientsPanelView::setContactBtnColor(int r, int g, int b, int a)
{
    Evas_Object *icon = elm_object_part_content_get(m_pContactBtn, "button_center_part");
    evas_object_color_set(icon, r, g, b, a);
}

void ConvRecipientsPanelView::deleteNextRecipient()
{
    if(!isMbeEmpty())
    {
        Elm_Object_Item *selectedItem = elm_multibuttonentry_selected_item_get(*m_pMbe);
        if(selectedItem)
            elm_object_item_del(selectedItem);

        selectLastItem();
    }
}

void ConvRecipientsPanelView::selectLastItem()
{
    Elm_Object_Item *lastItem = elm_multibuttonentry_last_item_get(*m_pMbe);

    if(lastItem)
    {
        elm_object_focus_allow_set(*m_pMbe, true);
        elm_multibuttonentry_item_selected_set(lastItem, true);
    }
    else
    {
        showMbe(false);
    }
}

bool ConvRecipientsPanelView::isEntryEmpty() const
{
    return elm_entry_is_empty(m_pEntry);
}

void ConvRecipientsPanelView::unselectMbeItem()
{
    Elm_Object_Item *selectedItem = elm_multibuttonentry_selected_item_get(*m_pMbe);
    if(selectedItem)
        elm_multibuttonentry_item_selected_set(selectedItem, EINA_FALSE);
}

unsigned int ConvRecipientsPanelView::getItemsCount() const
{
    unsigned int res = 0;
    const Eina_List* items = elm_multibuttonentry_items_get(*m_pMbe);
    if(items)
        res = eina_list_count(items);
    return res;
}

void ConvRecipientsPanelView::collapseRecipients()
{
    showMbe(false);
    m_SavedRecipText = getEntryText();
    clearEntry();
    updateShortenedRecipients();
}

void ConvRecipientsPanelView::expandRecipients()
{
    showMbe(!isMbeEmpty());
    setEntryText(m_SavedRecipText);
}

void ConvRecipientsPanelView::updateShortenedRecipients()
{
    const auto &items = m_pMbe->getItems();
    std::string shortenedRecipients;
    if(items.size() >= 1)
    {
        shortenedRecipients = items[0]->getDispName();
        if(items.size() > 1)
            shortenedRecipients += " + " + std::to_string(items.size() - 1);
    }
    setEntryText(shortenedRecipients);
}

void ConvRecipientsPanelView::onEntryChanged(Evas_Object *obj, void *event_info)
{
    unselectMbeItem();
}

void ConvRecipientsPanelView::onEntryFocusChanged(Evas_Object *obj, void *event_info)
{
    unselectMbeItem();
    onEntryFocusChanged();
}

void ConvRecipientsPanelView::onEntryClicked(Evas_Object *obj, void *event_info)
{
    unselectMbeItem();
}

void ConvRecipientsPanelView::onEntryMaxlengthReached(Evas_Object *obj, void *event_info)
{

}

void ConvRecipientsPanelView::onKeyDown(Evas_Object *obj, void *event_info)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;
    if((strcmp(ev->keyname, "BackSpace") == 0) && isEntryEmpty())
        deleteNextRecipient();
    onKeyDown(ev);
}

void ConvRecipientsPanelView::onContactBtnPressed(Evas_Object *obj, void *event_info)
{
    setContactBtnColor(BUTTON_PRESSED_COLOR);
}

void ConvRecipientsPanelView::onContactBtnUnpressed(Evas_Object *obj, void *event_info)
{
    setContactBtnColor(BUTTON_COLOR);
}

void ConvRecipientsPanelView::onContactBtnClicked(Evas_Object *obj, void *event_info)
{
    onContactButtonClicked();
}

MbeRecipientItem *ConvRecipientsPanelView::getSelectedItem() const
{
    return m_pMbe ? m_pMbe->getSelectedItem() : nullptr;
}
