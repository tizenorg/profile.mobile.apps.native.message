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

#include "ListItem.h"
#include "ListView.h"
#include "CallbackAssist.h"

// Std:
#include <assert.h>
#include <string.h>

using namespace Msg;

// ListItemClass :
ListItemStyle::ListItemStyle(const char *itemStyle, const char *decorateItemStyle, const char *decorateAllItemStyle)
    : m_pGenlistItemClass(nullptr)
{
    m_pGenlistItemClass = elm_genlist_item_class_new();

    setStyle(itemStyle, decorateItemStyle, decorateAllItemStyle);
    registerCallback();
}

ListItemStyle::~ListItemStyle()
{
    unregisterCallback();
    if(m_pGenlistItemClass)
        elm_genlist_item_class_free(m_pGenlistItemClass);
}

ListItemStyleRef ListItemStyle::create(const char *itemStyle,
                               const char *decorateItemStyle,
                               const char *decorateAllItemStyle)
{
    return std::shared_ptr<ListItemStyle>(new ListItemStyle(itemStyle, decorateItemStyle, decorateAllItemStyle));
}

void ListItemStyle::setStyle(const char * itemStyle, const char * decorateItemStyle, const char * decorateAllItemStyle)
{
    if(!m_pGenlistItemClass)
        return;

    m_pGenlistItemClass->decorate_all_item_style  = decorateAllItemStyle;
    m_pGenlistItemClass->decorate_item_style = decorateItemStyle;
    m_pGenlistItemClass->item_style = itemStyle;
}

void ListItemStyle::registerCallback()
{
    if(!m_pGenlistItemClass)
        return;

    m_pGenlistItemClass->func.text_get = &ListItem::elm_gen_item_text_get_cb;
    m_pGenlistItemClass->func.state_get = &ListItem::elm_gen_item_state_get_cb;
    m_pGenlistItemClass->func.del = &ListItem::elm_gen_item_del_cb;
    m_pGenlistItemClass->func.content_get = &ListItem::elm_gen_item_content_get_cb;
}

void ListItemStyle::unregisterCallback()
{
    if(!m_pGenlistItemClass)
        return;

    m_pGenlistItemClass->func.text_get = nullptr;
    m_pGenlistItemClass->func.state_get = nullptr;
    m_pGenlistItemClass->func.del = nullptr;
    m_pGenlistItemClass->func.content_get = nullptr;
}

// ListItemClass:
ListItem::ListItem(const ListItemStyleRef &itemStyle, Elm_Genlist_Item_Type type)
    : ViewItem()
    , m_pOwner(nullptr)
    , m_ItemStyle(itemStyle)
    , m_Type(type)
{
}

ListItem::ListItem(Elm_Genlist_Item_Type type)
    : ViewItem()
    , m_pOwner(nullptr)
    , m_ItemStyle()
    , m_Type(type)
    , m_Checked(false)
{

}

ListItem::~ListItem()
{
}

ListView *ListItem::getOwner() const
{
    return m_pOwner;
}

Elm_Genlist_Item_Type ListItem::getType() const
{
    return m_Type;
}

void ListItem::setStyle(ListItemStyleRef &style)
{
    m_ItemStyle = style;
}

const ListItemStyleRef &ListItem::getStyle() const
{
    return m_ItemStyle;
}

char *ListItem::elm_gen_item_text_get_cb(void *data, Evas_Object *obj, const char *part)
{
    ListItem *self = static_cast<ListItem*>(data);

    if(self)
    {
        std::string text = self->getText(*self, part);
        return text.empty() ? nullptr : strdup(text.c_str());
    }

    return nullptr;
}

Evas_Object *ListItem::elm_gen_item_content_get_cb(void * data, Evas_Object * obj, const char * part)
{
    ListItem *self = static_cast<ListItem*>(data);

    if(!self)
    {
        return nullptr;
    }

    if(self->getOwner()->getCheckMode())
    {
        const char *checkPart = self->getCheckPart(*self);
        if(checkPart && strcmp(checkPart, part) == 0)
        {
            return self->addCheck();
        }
    }

    return self->getContent(*self, part);
}

Evas_Object *ListItem::addCheck()
{
    Evas_Object *check = elm_check_add(*getOwner());
    elm_check_state_set(check, m_Checked);
    evas_object_propagate_events_set(check, EINA_FALSE);
    evas_object_smart_callback_add(check, "changed", SMART_CALLBACK(ListItem, onCheckChanged), this);
    elm_object_focus_allow_set(check, false);
    evas_object_show(check);
    return check;
}

void ListItem::changeCheckedState(bool updateUi)
{
    setCheckedState(!getCheckedState(), updateUi);
}

void ListItem::setCheckedState(bool state, bool updateUi)
{
    m_Checked = state;
    if(updateUi)
        elm_genlist_item_update(getElmObjItem());
}

bool ListItem::getCheckedState() const
{
    return m_Checked;
}

bool ListItem::isCheckable() const
{
    ListItem &self = const_cast<ListItem&>(*this);
    return self.getCheckPart(self) != nullptr;
}

void ListItem::onCheckChanged(Evas_Object *obj, void *eventInfo)
{
    m_Checked = elm_check_state_get(obj);
    getOwner()->notifyListener(nullptr, obj, getElmObjItem(), &IListViewListener::onListItemChecked);
}

Eina_Bool ListItem::elm_gen_item_state_get_cb(void *data, Evas_Object *obj, const char *part)
{
    ListItem *self = static_cast<ListItem*>(data);
    return self ? self->getState(*self, part) : false;
}

void ListItem::setSelected(bool selected)
{
    elm_genlist_item_selected_set(getElmObjItem(), selected);
}

bool ListItem::getSelected() const
{
    return elm_genlist_item_selected_get(getElmObjItem());
}

void ListItem::setExpanded(bool expand)
{
    elm_genlist_item_expanded_set(getElmObjItem(), expand);
}

void ListItem::update()
{
    if(m_ItemStyle->m_pGenlistItemClass != elm_genlist_item_item_class_get(getElmObjItem()))
        elm_genlist_item_item_class_update(getElmObjItem(), m_ItemStyle->m_pGenlistItemClass);
    elm_genlist_item_update(getElmObjItem());
}

void ListItem::updateFields(const char *parts, Elm_Genlist_Item_Field_Type type)
{
    elm_genlist_item_fields_update(getElmObjItem(), parts, type);
}

std::string ListItem::getText(ListItem &item, const char* part)
{
    return std::string();
}

Evas_Object* ListItem::getContent(ListItem &item, const char* part)
{
    return nullptr;
}

bool ListItem::getState(ListItem &item, const char* part)
{
    return false;
}

void ListItem::onDelete(ListItem &item)
{
    delete this;
}

const char *ListItem::getCheckPart(ListItem &item)
{
    return nullptr;
}

void ListItem::onViewDestroyed()
{
}

void ListItem::elm_gen_item_del_cb(void *data, Evas_Object *obj)
{
    ListItem *self = static_cast<ListItem*>(data);

    if(self)
        self->onDelete(*self);
}

