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

#include "ViewItem.h"

using namespace Msg;

ViewItem::ViewItem()
    : m_pItem(nullptr)
{
}

ViewItem::~ViewItem()
{
    elm_object_item_del_cb_set(m_pItem, nullptr);
}

void ViewItem::setElmObjItem(Elm_Object_Item *item)
{
    m_pItem = item;
    setData(this);
    elm_object_item_del_cb_set(m_pItem, on_delete_cb);
    onAttached(*this);
}

void ViewItem::destroy()
{
    elm_object_item_del(m_pItem);
}

void ViewItem::on_delete_cb(void *data, Evas_Object *obj, void *event_info)
{
    Elm_Object_Item *it = static_cast<Elm_Object_Item*>(event_info);
    ViewItem *self = static_cast<ViewItem*>(elm_object_item_data_get(it));
    self->onViewDestroyed();
}

Evas_Object *ViewItem::setContent(Evas_Object *content, const char *part, bool saveOldContent)
{
    Evas_Object *oldContent = saveOldContent ? elm_object_item_part_content_unset(m_pItem, part) : nullptr;
    elm_object_item_part_content_set(m_pItem, part, content);
    return oldContent;
}

Evas_Object *ViewItem::getContent(const char *part) const
{
    return m_pItem ? elm_object_item_part_content_get(m_pItem, part) : nullptr;
}

void ViewItem::emitSignal(const char *signal, const char *source)
{
    elm_object_item_signal_emit(m_pItem, signal, source);
}

