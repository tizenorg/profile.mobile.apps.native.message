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

#include "RecipientPanel.h"
#include "CallbackAssist.h"
#include "ResourceUtils.h"
#include "Logger.h"

#include <efl_extension.h>

using namespace Msg;

RecipientPanel::RecipientPanel(Evas_Object *parent, int entryMaxCharCount)
    : View()
    , m_pListener(nullptr)
    , m_pLayout(nullptr)
    , m_pMbe(nullptr)
    , m_pEntry(nullptr)
    , m_pContactBtn(nullptr)
    , m_pRecipRect(nullptr)
    , m_EntryMaxCharCount(entryMaxCharCount)
{
    create(parent);
}

RecipientPanel::~RecipientPanel()
{

}

void RecipientPanel::setListener(IRecipientPanelListener *listener)
{
    m_pListener = listener;
}

void RecipientPanel::appendItem(RecipientViewItem &item)
{
    item.m_pOwner = this;
    Elm_Object_Item *elmItem = elm_multibuttonentry_item_append(m_pMbe, item.m_DisplayName.c_str(),
                                                                SMART_CALLBACK(RecipientPanel, onItemPressed), &item);
    if(elmItem)
        item.setElmObjItem(elmItem);
    else
        item.m_pOwner = nullptr;
}

bool RecipientPanel::getEntryFocus() const
{
    return elm_object_focus_get(m_pEntry);
}

void RecipientPanel::setEntryFocus(bool val)
{
    elm_object_focus_set(m_pEntry, val);
}

void RecipientPanel::clearEntry()
{
    elm_object_text_set(m_pEntry, "");
}

void RecipientPanel::setGuideText(const char *text)
{
    elm_object_part_text_set(m_pEntry, "elm.guide", text);
}

RecipientViewItemList RecipientPanel::getItems() const
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

void RecipientPanel::showMbe(bool show)
{
    const char *prog = show ? "show_to_mbe" : "hide_to_mbe";
    elm_object_signal_emit(m_pLayout, prog, "*");
}

bool RecipientPanel::hasItems()const
{
    return elm_multibuttonentry_first_item_get(m_pMbe) != nullptr;
}

std::string RecipientPanel::getEntryText() const
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

void RecipientPanel::setEntryText(const char *utf8)
{
    char *text = elm_entry_utf8_to_markup(utf8);

    if(text)
    {
        elm_entry_entry_set(m_pEntry, text);
        elm_entry_cursor_end_set(m_pEntry);
        free(text);
    }
}

void RecipientPanel::create(Evas_Object *parent)
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

    m_pMbe = mbeCreate(m_pLayout);
    m_pEntry = entryCreate(m_pLayout);
    m_pContactBtn = createContactBtn(m_pLayout);

    elm_object_part_content_set(m_pLayout, "swl.mbe_scroll", m_pMbe);
    elm_object_part_content_set(m_pLayout, "swl.entry", m_pEntry);
    elm_object_part_content_set(m_pLayout, "swl.contact_btn", m_pContactBtn);
}

Evas_Object *RecipientPanel::mbeCreate(Evas_Object *parent)
{
    Evas_Object *mbe = elm_multibuttonentry_add(parent);
    elm_multibuttonentry_editable_set(mbe, EINA_FALSE);
    elm_multibuttonentry_expanded_set(mbe, EINA_TRUE);
    evas_object_size_hint_weight_set(mbe, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(mbe, EVAS_HINT_FILL, 0.0);
    elm_object_focus_allow_set(mbe, EINA_FALSE);
    elm_object_tree_focus_allow_set(mbe, EINA_TRUE);
    evas_object_show(mbe);

  // evas_object_smart_callback_add(mbe, "item,added", SMART_CALLBACK(RecipientPanel, onItemAdded), this);
    evas_object_smart_callback_add(mbe, "item,selected", SMART_CALLBACK(RecipientPanel, onItemSelected), this);
    evas_object_smart_callback_add(mbe, "item,deleted", SMART_CALLBACK(RecipientPanel, onItemDeleted), this);
    evas_object_smart_callback_add(mbe, "item,clicked", SMART_CALLBACK(RecipientPanel, onItemClicked), this);

    evas_object_smart_callback_add(mbe, "focused", SMART_CALLBACK(RecipientPanel, onMbeFocused), this);
    evas_object_smart_callback_add(mbe, "clicked", SMART_CALLBACK(RecipientPanel, onMbeClicked), this);
    evas_object_smart_callback_add(mbe, "unfocused", SMART_CALLBACK(RecipientPanel, onMbeUnfocused), this);

    return mbe;
}

Evas_Object *RecipientPanel::entryCreate(Evas_Object *parent)
{
    Evas_Object *entry = elm_entry_add(parent);
    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_object_signal_emit(entry, "elm,action,hide,search_icon", "");
    elm_entry_editable_set(entry, EINA_TRUE);
    elm_entry_single_line_set(entry, EINA_TRUE);
    elm_entry_scrollable_set(entry, EINA_TRUE);
    eext_entry_selection_back_event_allow_set(entry, EINA_TRUE);
    elm_scroller_policy_set(entry, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
    elm_entry_autocapital_type_set(entry, ELM_AUTOCAPITAL_TYPE_NONE);
    elm_entry_input_panel_return_key_type_set(entry, ELM_INPUT_PANEL_RETURN_KEY_TYPE_NEXT);

    Elm_Entry_Filter_Limit_Size limitFilter = {};
    limitFilter.max_char_count = m_EntryMaxCharCount;
    elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limitFilter);

    evas_object_show(entry);

    /* keypad is manually controlled, so disabling the auto control of keypad */
    //elm_entry_input_panel_enabled_set(entry, EINA_FALSE);

    //Ecore_IMF_Context *context = elm_entry_imf_context_get(entry);
   // ecore_imf_context_input_panel_event_callback_add((Ecore_IMF_Context *)context, ECORE_IMF_INPUT_PANEL_STATE_EVENT, msg_ui_composer_entry_imf_state_cb, rd->cd);
   // ecore_imf_context_input_panel_event_callback_add((Ecore_IMF_Context *)context, ECORE_IMF_INPUT_PANEL_GEOMETRY_EVENT, msg_ui_composer_entry_imf_resize_cb, rd->cd);

    evas_object_smart_callback_add(entry, "changed", SMART_CALLBACK(RecipientPanel, onEntryChanged), this);
    evas_object_smart_callback_add(entry, "preedit,changed", SMART_CALLBACK(RecipientPanel, onEntryPreeditChanged), this);
    evas_object_smart_callback_add(entry, "activated", SMART_CALLBACK(RecipientPanel, onEntryActivated), this);
    evas_object_smart_callback_add(entry, "focused", SMART_CALLBACK(RecipientPanel, onEntryFocusChanged), this);
    evas_object_smart_callback_add(entry, "unfocused", SMART_CALLBACK(RecipientPanel, onEntryFocusChanged), this);
    evas_object_smart_callback_add(entry, "clicked", SMART_CALLBACK(RecipientPanel, onEntryClicked), this);
    evas_object_smart_callback_add(entry, "maxlength,reached", SMART_CALLBACK(RecipientPanel, onEntryMaxlengthReached), this);
    evas_object_event_callback_add(entry, EVAS_CALLBACK_KEY_DOWN, EVAS_EVENT_CALLBACK(RecipientPanel, onKeyDown), this);
    evas_object_event_callback_add(entry, EVAS_CALLBACK_RESIZE, EVAS_EVENT_CALLBACK(RecipientPanel, onEntryGeometryChanged), this);
    evas_object_event_callback_add(entry, EVAS_CALLBACK_MOVE, EVAS_EVENT_CALLBACK(RecipientPanel, onEntryGeometryChanged), this);

 //   evas_object_event_callback_add(entry, EVAS_CALLBACK_KEY_UP, msg_ui_recipient_entry_key_up_cb, rd->cd);

    return entry;
}

Evas_Object *RecipientPanel::createContactBtn(Evas_Object *parent)
{
    m_pContactBtn = elm_button_add(parent);
    elm_object_style_set(m_pContactBtn, "contact_button");

    Evas_Object *icon = elm_image_add(parent);
    std::string imgPath = ResourceUtils::getResourcePath(IMAGES_EDJ_PATH);
    elm_image_file_set(icon, imgPath.c_str(), CONTACT_IMG);

    elm_object_part_content_set(m_pContactBtn, "button_center_part", icon);
    elm_image_resizable_set(icon, EINA_TRUE, EINA_TRUE);
    evas_object_color_set(icon, BUTTON_COLOR);

    evas_object_smart_callback_add(m_pContactBtn, "pressed", SMART_CALLBACK(RecipientPanel, onContactBtnPressed), this);
    evas_object_smart_callback_add(m_pContactBtn, "unpressed", SMART_CALLBACK(RecipientPanel, onContactBtnUnpressed), this);
    evas_object_smart_callback_add(m_pContactBtn, "clicked", SMART_CALLBACK(RecipientPanel, onContactBtnClicked), this);

    setContactBtnColor(BUTTON_COLOR);

    return m_pContactBtn;
}

void RecipientPanel::setRecipientRect(Evas_Object *rect)
{
    m_pRecipRect = rect;
}

void RecipientPanel::onEntryGeometryChanged(Evas_Object *obj, void *event_info)
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

void RecipientPanel::setContactBtnColor(int r, int g, int b, int a)
{
    Evas_Object *icon = elm_object_part_content_get(m_pContactBtn, "button_center_part");
    evas_object_color_set(icon, r, g, b, a);
}

RecipientViewItem *RecipientPanel::getItem(void *data)
{
    Elm_Object_Item *elmItem = (Elm_Object_Item*)data;
    RecipientViewItem* it = static_cast<RecipientViewItem*>(elm_object_item_data_get(elmItem));

    if(!it)
        MSG_LOG_ERROR("Item is null");

    return it;
}

void RecipientPanel::onItemSelected(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    if(m_pListener && it)
        m_pListener->onItemSelected(*this, *it);
}

void RecipientPanel::onItemDeleted(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    delete it;
}

void RecipientPanel::onItemClicked(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    if(m_pListener && it)
        m_pListener->onItemClicked(*this, *it);
}

void RecipientPanel::onItemPressed(Evas_Object *obj, void *item)
{
    RecipientViewItem* it = getItem(item);
    if(m_pListener && it)
        m_pListener->onItemPressed(*this, *it);
}

Eina_Bool RecipientPanel::onMbeFilter(Evas_Object *obj, const char *item_label, const void *item_data)
{
    return EINA_TRUE;
}

void RecipientPanel::onMbeFocused(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onMbeUnfocused(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onMbeClicked(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onEntryChanged(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onEntryPreeditChanged(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onEntryActivated(Evas_Object *obj, void *event_info)
{

}


void RecipientPanel::onEntryFocusChanged(Evas_Object *obj, void *event_info)
{
    if(m_pListener)
        m_pListener->onEntryFocusChanged(*this);
}

void RecipientPanel::onEntryClicked(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onEntryMaxlengthReached(Evas_Object *obj, void *event_info)
{

}

void RecipientPanel::onKeyDown(Evas_Object *obj, void *event_info)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event_info;

    if(m_pListener)
        m_pListener->onKeyDown(*this, ev);
}

void RecipientPanel::onContactBtnPressed(Evas_Object *obj, void *event_info)
{
    setContactBtnColor(BUTTON_PRESSED_COLOR);
}

void RecipientPanel::onContactBtnUnpressed(Evas_Object *obj, void *event_info)
{
    setContactBtnColor(BUTTON_COLOR);
}

void RecipientPanel::onContactBtnClicked(Evas_Object *obj, void *event_info)
{
    if(m_pListener)
        m_pListener->onContactButtonClicked(*this);
}
