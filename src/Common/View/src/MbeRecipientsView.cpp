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

#include "MbeRecipientsView.h"
#include "CallbackAssist.h"

using namespace Msg;

MbeRecipientsView::MbeRecipientsView(Evas_Object *parent)
{
    setEo(elm_multibuttonentry_add(parent));
    elm_multibuttonentry_editable_set(getEo(), EINA_FALSE);
    elm_multibuttonentry_expanded_set(getEo(), EINA_TRUE);
    elm_object_focus_allow_set(getEo(), EINA_FALSE);
    elm_object_tree_focus_allow_set(getEo(), EINA_TRUE);
}

MbeRecipientsView::~MbeRecipientsView()
{

}

void MbeRecipientsView::appendItem(MbeRecipientItem &item)
{
    Elm_Object_Item *elmItem = elm_multibuttonentry_item_append(getEo(), item.getDispName().c_str(), nullptr, &item);
    item.setElmObjItem(elmItem);
}

int MbeRecipientsView::getItemsCount() const
{
    unsigned int res = 0;
    const Eina_List* items = elm_multibuttonentry_items_get(getEo());
    if(items)
        res = eina_list_count(items);
    return res;
}

std::vector<MbeRecipientItem*> MbeRecipientsView::getItems() const
{
    std::vector<MbeRecipientItem*> res;

    Elm_Object_Item *elmItem = elm_multibuttonentry_first_item_get(getEo());
    while(elmItem)
    {
        MbeRecipientItem *item = static_cast<MbeRecipientItem*>(elm_object_item_data_get(elmItem));
        res.push_back(item);
        elmItem = elm_multibuttonentry_item_next_get(elmItem);
    }

    return res;
}

MbeRecipientItem *MbeRecipientsView::getSelectedItem() const
{
    return ViewItem::staticCast<MbeRecipientItem*>(elm_multibuttonentry_selected_item_get(getEo()));
}

void MbeRecipientsView::selectItem(MbeRecipientItem &item, bool select)
{
    elm_object_focus_allow_set(item, true);
    elm_multibuttonentry_item_selected_set(item, select);
}

void MbeRecipientsView::unselect()
{
    Elm_Object_Item *selectedItem = elm_multibuttonentry_selected_item_get(getEo());
    if(selectedItem)
        elm_multibuttonentry_item_selected_set(selectedItem, false);
}

bool MbeRecipientsView::isEmpty() const
{
    return elm_multibuttonentry_first_item_get(getEo()) == nullptr;
}

void MbeRecipientsView::clear()
{
    elm_multibuttonentry_clear(getEo());
}


