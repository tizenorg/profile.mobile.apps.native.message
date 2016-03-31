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

#include "ConvContactListView.h"
#include "Resource.h"

using namespace Msg;

ConvContactListView::ConvContactListView(Evas_Object *parent)
    : m_pRect(nullptr)
    , m_pBox(nullptr)
    , m_pList(nullptr)
{
    setEo(addLayout(parent, CONV_CONTACT_LIST_LAYOUT, "conv_contact_list_layout"));
    setFocusAllow(false);

    m_pList = new ListView(getEo());
    m_pList->setMultiSelection(true);
    m_pList->setMode(ELM_LIST_COMPRESS);
    m_pList->setHomogeneous(true);
    m_pList->setFocusAllow(false);
    m_pList->addSmartCb("realized", SMART_CALLBACK(ConvContactListView, onGeometryChanged), this);
    m_pList->addSmartCb("unrealized", SMART_CALLBACK(ConvContactListView, onGeometryChanged), this);
    m_pList->expand();
    m_pList->show();

    m_pBox = elm_box_add(getEo());
    evas_object_show(m_pBox);
    elm_box_pack_end(m_pBox, *m_pList);

    setContent(m_pBox, "swl.contact_list");
    setContent(createRect(getEo()), "swl.rect");
}

ConvContactListView::~ConvContactListView()
{

}

ListView &ConvContactListView::getList()
{
    return *m_pList;
}

const ListView &ConvContactListView::getList() const
{
    return *m_pList;
}

void ConvContactListView::recalcGeometry()
{
    int itemHeight = getItemHeight();

    int h = itemHeight * m_pList->getItemsCount();

    int maxListHeight = 0;
    evas_object_geometry_get(m_pRect, nullptr, nullptr, nullptr, &maxListHeight);

    if(h > maxListHeight)
        h = maxListHeight;

    evas_object_size_hint_min_set(m_pBox, 0, h);
    evas_object_size_hint_max_set(m_pBox, -1, h);
}

int ConvContactListView::getItemHeight() const
{
    int res = 0;
    Evas_Object *track = nullptr;
    Elm_Object_Item *item = elm_genlist_first_item_get(*m_pList);

    for(; item ;)
    {
        track = elm_object_item_track(item);
        if(track)
            break;
        item = elm_genlist_item_next_get(item);
    }

    if(track)
    {
        evas_object_geometry_get(track, nullptr, nullptr, nullptr, &res);
        elm_object_item_untrack(track);
    }

    return res;
}

Evas_Object *ConvContactListView::createRect(Evas_Object *parent)
{
    m_pRect = evas_object_rectangle_add(evas_object_evas_get(parent));
    evas_object_show(m_pRect);
    evas_object_color_set(m_pRect, 0, 0, 0, 0);
    evas_object_event_callback_add(m_pRect, EVAS_CALLBACK_MOVE, EVAS_EVENT_CALLBACK(ConvContactListView, onGeometryChanged), this);
    evas_object_event_callback_add(m_pRect, EVAS_CALLBACK_RESIZE, EVAS_EVENT_CALLBACK(ConvContactListView, onGeometryChanged), this);
    evas_object_event_callback_add(m_pRect, EVAS_CALLBACK_CHANGED_SIZE_HINTS, EVAS_EVENT_CALLBACK(ConvContactListView, onGeometryChanged), this);
    return m_pRect;
}

void ConvContactListView::onGeometryChanged(Evas_Object *obj, void *eventInfo)
{
    recalcGeometry();
}

