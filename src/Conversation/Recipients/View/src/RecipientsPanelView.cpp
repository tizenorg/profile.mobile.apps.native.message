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

#include "RecipientsPanelView.h"
#include "CallbackAssist.h"
#include "ResourceUtils.h"
#include "Logger.h"
#include "LangUtils.h"

#include <efl_extension.h>

using namespace Msg;

RecipientsPanelView::RecipientsPanelView(Evas_Object *parent, int entryMaxCharCount)
    : View()
    , m_pLayout(nullptr)
    , m_pMbe(nullptr)
    , m_pEntry(nullptr)
    , m_pContactBtn(nullptr)
    , m_pRecipRect(nullptr)
    , m_EntryMaxCharCount(entryMaxCharCount)
{
    create(parent);
}

RecipientsPanelView::~RecipientsPanelView()
{

}

void RecipientsPanelView::appendItem(RecipientViewItem &item)
{
    item.m_pOwner = this;
    Elm_Object_Item *elmItem = elm_multibuttonentry_item_append(m_pMbe, item.getDisplayName().c_str(), &onItemPressed, &item);

    if(elmItem)
    	item.setElmObjItem(elmItem);
    else
        item.m_pOwner = nullptr;
}

bool RecipientsPanelView::getEntryFocus() const
{
    return elm_object_focus_get(m_pEntry);
}

void RecipientsPanelView::setEntryFocus(bool val)
{
    elm_object_focus_set(m_pEntry, val);
}

void RecipientsPanelView::clearEntry()
{
    elm_object_text_set(m_pEntry, "");
}

RecipientViewItemList RecipientsPanelView::getItems() const
{
    RecipientViewItemList res;

    Elm_Object_Item *elmItem = elm_multibuttonentry_first_item_get(m_pMbe);
    while(elmItem)
    {
        RecipientViewItem *item = static_cast<RecipientViewItem*>(elm_object_item_data_get(elmItem));
        res.push_back(item);
        elmItem = elm_multibuttonentry_item_next_get(elmItem);
    }

    return res;
}

void RecipientsPanelView::showMbe(bool show)
{
    const char *prog = show ? "show_to_mbe" : "hide_to_mbe";
    elm_object_signal_emit(m_pLayout, prog, "*");
}

bool RecipientsPanelView::isMbeEmpty()const
{
    return elm_multibuttonentry_first_item_get(m_pMbe) == nullptr;
}

std::string RecipientsPanelView::getEntryText() const
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

void RecipientsPanelView::setEntryText(const std::string &utf8)
{
    char *text = elm_entry_utf8_to_markup(utf8.c_str());

    if(text)
    {
        elm_entry_entry_set(m_pEntry, text);
        elm_entry_cursor_end_set(m_pEntry);
        free(text);
    }
}

void RecipientsPanelView::create(Evas_Object *parent)
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

    std::string path = ResourceUtils::getResourcePath(RECIPIENT_PANEL_EDJ_PATH);
    elm_layout_file_set(m_pLayout, path.c_str(), "recipient_panel");

    Evas_Object *mbe = m_pMbe = mbeCreate(m_pLayout);
    Evas_Object *entry =  entryCreate(m_pLayout);
    Evas_Object *button = createContactBtn(m_pLayout);

    elm_object_part_content_set(m_pLayout, "swl.mbe_scroll", mbe);
    elm_object_part_content_set(m_pLayout, "swl.entry", entry);
    elm_object_part_content_set(m_pLayout, "swl.contact_btn", button);
}

Evas_Object *RecipientsPanelView::mbeCreate(Evas_Object *parent)
{
    m_pMbe = elm_multibuttonentry_add(parent);
    elm_multibuttonentry_editable_set(m_pMbe, EINA_FALSE);
    elm_multibuttonentry_expanded_set(m_pMbe, EINA_TRUE);
    evas_object_size_hint_weight_set(m_pMbe, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(m_pMbe, EVAS_HINT_FILL, 0.0);
    elm_object_focus_allow_set(m_pMbe, EINA_FALSE);
    elm_object_tree_focus_allow_set(m_pMbe, EINA_TRUE);
    evas_object_show(m_pMbe);

    evas_object_smart_callback_add(m_pMbe, "item,added", SMART_CALLBACK(RecipientsPanelView, onItemAdded), this);
    evas_object_smart_callback_add(m_pMbe, "item,deleted", SMART_CALLBACK(RecipientsPanelView, onItemDeleted), this);
    evas_object_smart_callback_add(m_pMbe, "item,selected", SMART_CALLBACK(RecipientsPanelView, onItemSelected), this);
    evas_object_smart_callback_add(m_pMbe, "item,clicked", SMART_CALLBACK(RecipientsPanelView, onItemClicked), this);

    evas_object_smart_callback_add(m_pMbe, "focused", SMART_CALLBACK(RecipientsPanelView, onMbeFocused), this);
    evas_object_smart_callback_add(m_pMbe, "clicked", SMART_CALLBACK(RecipientsPanelView, onMbeClicked), this);
    evas_object_smart_callback_add(m_pMbe, "unfocused", SMART_CALLBACK(RecipientsPanelView, onMbeUnfocused), this);

    return m_pMbe;
}

Evas_Object *RecipientsPanelView::entryCreate(Evas_Object *parent)
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

    evas_object_show(m_pEntry);

    // TODO: check this:
    /* keypad is manually controlled, so disabling the auto control of keypad */
    //elm_entry_input_panel_enabled_set(entry, EINA_FALSE);

    // Ecore_IMF_Context *context = elm_entry_imf_context_get(entry);
    // ecore_imf_context_input_panel_event_callback_add((Ecore_IMF_Context *)context, ECORE_IMF_INPUT_PANEL_STATE_EVENT, msg_ui_composer_entry_imf_state_cb, rd->cd);
    // ecore_imf_context_input_panel_event_callback_add((Ecore_IMF_Context *)context, ECORE_IMF_INPUT_PANEL_GEOMETRY_EVENT, msg_ui_composer_entry_imf_resize_cb, rd->cd);

    evas_object_smart_callback_add(m_pEntry, "changed", SMART_CALLBACK(RecipientsPanelView, onEntryChanged), this);
    evas_object_smart_callback_add(m_pEntry, "preedit,changed", SMART_CALLBACK(RecipientsPanelView, onEntryPreeditChanged), this);
    evas_object_smart_callback_add(m_pEntry, "activated", SMART_CALLBACK(RecipientsPanelView, onEntryActivated), this);
    evas_object_smart_callback_add(m_pEntry, "focused", SMART_CALLBACK(RecipientsPanelView, onEntryFocusChanged), this);
    evas_object_smart_callback_add(m_pEntry, "unfocused", SMART_CALLBACK(RecipientsPanelView, onEntryFocusChanged), this);
    evas_object_smart_callback_add(m_pEntry, "clicked", SMART_CALLBACK(RecipientsPanelView, onEntryClicked), this);
    evas_object_smart_callback_add(m_pEntry, "maxlength,reached", SMART_CALLBACK(RecipientsPanelView, onEntryMaxlengthReached), this);
    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_KEY_DOWN, EVAS_EVENT_CALLBACK(RecipientsPanelView, onKeyDown), this);
    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_RESIZE, EVAS_EVENT_CALLBACK(RecipientsPanelView, onEntryGeometryChanged), this);
    evas_object_event_callback_add(m_pEntry, EVAS_CALLBACK_MOVE, EVAS_EVENT_CALLBACK(RecipientsPanelView, onEntryGeometryChanged), this);

    return m_pEntry;
}

Evas_Object *RecipientsPanelView::createContactBtn(Evas_Object *parent)
{
    m_pContactBtn = elm_button_add(parent);
    elm_object_style_set(m_pContactBtn, "contact_button");

    Evas_Object *icon = elm_image_add(parent);
    std::string imgPath = ResourceUtils::getResourcePath(IMAGES_EDJ_PATH);
    elm_image_file_set(icon, imgPath.c_str(), CONTACT_IMG);

    elm_object_part_content_set(m_pContactBtn, "button_center_part", icon);
    elm_image_resizable_set(icon, EINA_TRUE, EINA_TRUE);
    evas_object_color_set(icon, BUTTON_COLOR);

    evas_object_smart_callback_add(m_pContactBtn, "pressed", SMART_CALLBACK(RecipientsPanelView, onContactBtnPressed), this);
    evas_object_smart_callback_add(m_pContactBtn, "unpressed", SMART_CALLBACK(RecipientsPanelView, onContactBtnUnpressed), this);
    evas_object_smart_callback_add(m_pContactBtn, "clicked", SMART_CALLBACK(RecipientsPanelView, onContactBtnClicked), this);

    setContactBtnColor(BUTTON_COLOR);

    return m_pContactBtn;
}

void RecipientsPanelView::setRecipientRect(Evas_Object *rect)
{
    m_pRecipRect = rect;
}

void RecipientsPanelView::onEntryGeometryChanged(Evas_Object *obj, void *event_info)
{
    if(!m_pRecipRect)
        return;

    int y = 0;
    int w = 0;
    int h = 0;
    int lyayoutTop = 0;
    int entryBottom = 0;

    evas_object_geometry_get(m_pEntry, nullptr, &y, nullptr, &h);
    entryBottom = y + h;

    evas_object_geometry_get(m_pLayout, nullptr, &y, nullptr, nullptr);
    lyayoutTop = y;

    evas_object_size_hint_min_get(m_pRecipRect, &w, nullptr);
    evas_object_size_hint_min_set(m_pRecipRect, w, entryBottom - lyayoutTop);
}

void RecipientsPanelView::setContactBtnColor(int r, int g, int b, int a)
{
    Evas_Object *icon = elm_object_part_content_get(m_pContactBtn, "button_center_part");
    evas_object_color_set(icon, r, g, b, a);
}

RecipientViewItem *RecipientsPanelView::getItem(void *data)
{
    Elm_Object_Item *elmItem = (Elm_Object_Item*)data;
    RecipientViewItem* it = static_cast<RecipientViewItem*>(elm_object_item_data_get(elmItem));

    if(!it)
        MSG_LOG_ERROR("Item is null");

    return it;
}

void RecipientsPanelView::onItemSelected(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    onItemSelected(*it);
}

void RecipientsPanelView::onItemDeleted(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    onItemDeleted(*it);
    delete it;
}

void RecipientsPanelView::onItemAdded(Evas_Object *obj, void *item)
{
    onItemAdded(*getItem(item));
}

void RecipientsPanelView::onItemClicked(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    onItemClicked(*it);
}

void RecipientsPanelView::onItemPressed(void *data, Evas_Object *obj, void *eventInfo)
{
    RecipientViewItem* it = static_cast<RecipientViewItem*>(data);
    it->m_pOwner->onItemPressed(*it);
}

Eina_Bool RecipientsPanelView::onMbeFilter(Evas_Object *obj, const char *item_label, const void *item_data)
{
    return EINA_TRUE;
}

void RecipientsPanelView::onMbeFocused(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onMbeUnfocused(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onMbeClicked(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onEntryChanged(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onEntryPreeditChanged(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onEntryActivated(Evas_Object *obj, void *event_info)
{

}


void RecipientsPanelView::onEntryFocusChanged(Evas_Object *obj, void *event_info)
{
    onEntryFocusChanged();
}

void RecipientsPanelView::onEntryClicked(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onEntryMaxlengthReached(Evas_Object *obj, void *event_info)
{

}

void RecipientsPanelView::onKeyDown(Evas_Object *obj, void *event_info)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;
    onKeyDown(ev);
}

void RecipientsPanelView::onContactBtnPressed(Evas_Object *obj, void *event_info)
{
    setContactBtnColor(BUTTON_PRESSED_COLOR);
}

void RecipientsPanelView::onContactBtnUnpressed(Evas_Object *obj, void *event_info)
{
    setContactBtnColor(BUTTON_COLOR);
}

void RecipientsPanelView::onContactBtnClicked(Evas_Object *obj, void *event_info)
{
    onContactButtonClicked();
}
