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

#include "ListView.h"
#include "Logger.h"

// Std:
#include <assert.h>
#include <string.h>

using namespace Msg;

namespace
{
    inline ListItem *getItem(Elm_Object_Item *obj)
    {
        return obj ? static_cast<ListItem*>(elm_object_item_data_get(obj)) : nullptr;
    }
}

// ListView:
ListView::ListView(Evas_Object *parent)
    : View()
    , m_pListener(nullptr)
    , m_CheckMode(false)
{
    createListView(parent);
}

ListView::~ListView()
{
}

void ListView::setListener(IListViewListener *listener)
{
    m_pListener = listener;
}

void ListView::createListView(Evas_Object *parent)
{
    setEo(elm_genlist_add(parent));
    evas_object_smart_callback_add(getEo(), "drag,start,right", on_drag_right_cb, this);
    evas_object_smart_callback_add(getEo(), "drag,start,left", on_drag_left_cb, this);
}

bool ListView::appendItem(ListItem &listItem, ListItem *parent)
{
    Elm_Object_Item *parentItem = parent ? parent->getElmObjItem() : nullptr;

    listItem.m_pOwner = this;
    Elm_Object_Item *elmItem = elm_genlist_item_append(getEo(), listItem.m_ItemStyle->m_pGenlistItemClass,
                                                       &listItem, parentItem, listItem.getType(), on_item_selected_cb, this);
    listItem.setElmObjItem(elmItem);
    return elmItem != nullptr;
}

bool ListView::prependItem(ListItem &listItem, ListItem *parent)
{
    Elm_Object_Item *parentItem = parent ? parent->getElmObjItem() : nullptr;

    listItem.m_pOwner = this;
    Elm_Object_Item *elmItem = elm_genlist_item_prepend(getEo(), listItem.m_ItemStyle->m_pGenlistItemClass,
                                                       &listItem, parentItem, listItem.getType(), on_item_selected_cb, this);
    listItem.setElmObjItem(elmItem);
    return elmItem != nullptr;
}

void ListView::deleteItem(ListItem &listItem)
{
    elm_object_item_del(listItem.getElmObjItem());
}

ListItemCollection ListView::getItems() const
{
    ListItemCollection list;
    Elm_Object_Item *elmItem = elm_genlist_first_item_get(getEo());
    while(elmItem)
    {
        void *data = elm_object_item_data_get(elmItem);
        MSG_ASSERT(data, "Genlist item data is NULL");
        if(data)
        {
            ListItem *listItem = static_cast<ListItem *>(data);
            list.push_back(listItem);
        }
        elmItem = elm_genlist_item_next_get(elmItem);
    }

    return list;
}

ListItem *ListView::getFirstItem() const
{
    return getItem(elm_genlist_first_item_get(getEo()));
}

ListItem *ListView::getLastItem() const
{
    return getItem(elm_genlist_last_item_get(getEo()));
}

void ListView::showItem(ListItem &listItem, Elm_Genlist_Item_Scrollto_Type type)
{
    elm_genlist_item_show(listItem.getElmObjItem(), type);
}

void ListView::clear()
{
    elm_genlist_clear(getEo());
}

void ListView::setMode(Elm_List_Mode mode)
{
    elm_genlist_mode_set(getEo(), mode);
}

ListItem *ListView::getSelectedItem() const
{
    Elm_Object_Item *elmItem = elm_genlist_selected_item_get(getEo());
    return static_cast<ListItem*>(elm_object_item_data_get(elmItem));
}

void ListView::notifyListener(void *data, Evas_Object *obj, void *event_info, ListenerMethod method)
{
    Elm_Object_Item *elmItem = static_cast<Elm_Object_Item*>(event_info);
    ListItem *item = static_cast<ListItem*>(elm_object_item_data_get(elmItem));

    MSG_ASSERT(item, "Item is null");
    if(item)
    {
        ListView *owner = item->getOwner();
        MSG_ASSERT(owner, "ListItem::owner is null");

        if(owner && owner->m_pListener)
            (owner->m_pListener->*method)(*item, data);
    }
}

void ListView::setCheckMode(bool check)
{
    m_CheckMode = check;
}

bool ListView::getCheckMode() const
{
    return m_CheckMode;
}

void ListView::checkAllItems(bool check)
{
    ListItemCollection items = getItems();
    for(ListItem *item: items)
    {
        // TODO: Optimization, update UI only for visible items
        item->setCheckedState(check, true);
    }
}

void ListView::updateAllItems()
{
    elm_genlist_realized_items_update(getEo());
}

void ListView::setMultiSelection(bool value)
{
    elm_genlist_multi_select_set(getEo(), value);
}

bool ListView::getMultiSelection() const
{
    return elm_genlist_multi_select_get(getEo());
}

void ListView::registerItemRealizedCallback()
{
    evas_object_smart_callback_add(getEo(), "realized", ListView::on_realized_cb, this);
}

void ListView::registerItemExpandedCallback()
{
    evas_object_smart_callback_add(getEo(), "expanded", ListView::on_expanded_cb, this);
}

void ListView::registerItemContractedCallback()
{
    evas_object_smart_callback_add(getEo(), "contracted", ListView::on_contracted_cb, this);
}

void ListView::on_item_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
    Elm_Object_Item *elmItem = static_cast<Elm_Object_Item*>(event_info);
    ListItem *self = static_cast<ListItem*>(elm_object_item_data_get(elmItem));

    if(self->getOwner()->getCheckMode() && self->isCheckable())
    {
        self->setSelected(false);
        self->changeCheckedState(true);
        notifyListener(data, obj, event_info, &IListViewListener::onListItemChecked);
    }
    else
    {
        notifyListener(data, obj, event_info, &IListViewListener::onListItemSelected);
    }
}

void ListView::on_drag_left_cb(void *data, Evas_Object *obj, void *event_info)
{
    notifyListener(data, obj, event_info, &IListViewListener::onListItemDragLeft);
}

void ListView::on_drag_right_cb(void *data, Evas_Object *obj, void *event_info)
{
    notifyListener(data, obj, event_info, &IListViewListener::onListItemDragRight);
}

void ListView::on_realized_cb(void *data, Evas_Object *obj, void *event_info)
{
    notifyListener(data, obj, event_info, &IListViewListener::onListItemRealized);
}

void ListView::on_expanded_cb(void *data, Evas_Object *obj, void *event_info)
{
    notifyListener(data, obj, event_info, &IListViewListener::onListItemExpanded);
}

void ListView::on_contracted_cb(void *data, Evas_Object *obj, void *event_info)
{
    notifyListener(data, obj, event_info, &IListViewListener::onListItemContracted);
}
