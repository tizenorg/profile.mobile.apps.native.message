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
#include "TextPageViewItem.h"
#include "ImagePageViewItem.h"

#include <efl_extension.h>

using namespace Msg;

namespace
{
    const char *pageGroup = "conv/body/page";
}

PageView::PageView(BodyView &parent)
    : BodyViewItem(parent, PageType)
    , m_Body(parent)
    , m_pBox(nullptr)
{
    Evas_Object *layout = createLayout(m_Body);
    Evas_Object *box = createBox(layout);
    elm_object_part_content_set(layout, "swl.page", box);
    BodyViewItem::setChild(layout);
}

PageView::~PageView()
{

}

bool PageView::isEmpty() const
{
    for(auto it : m_PageItemMap)
    {
        if(!it.second->isEmpty())
        {
            return false;
        }
    }
    return true;
}

Evas_Object *PageView::getItemParent() const
{
    return m_pBox;
}

void PageView::addItem(PageViewItem &item)
{
    auto it = m_PageItemMap.find(item.getType());
    if(it != m_PageItemMap.end())
    {
        if(it->second == &item)
        {
            return;
        }
        else
        {
            removeItem(item.getType());
        }
    }

    item.expand();
    item.show();
    elm_box_pack_end(m_pBox, item);
    m_PageItemMap[item.getType()] = &item;
}

void PageView::removeItem(PageViewItem::Type type)
{
    auto it = m_PageItemMap.find(type);
    if(it != m_PageItemMap.end())
    {
        it->second->destroy();
        m_PageItemMap.erase(it);
    }
}

PageViewItem *PageView::getItem(PageViewItem::Type type) const
{
    auto it = m_PageItemMap.find(type);
    return it == m_PageItemMap.end() ? nullptr : it->second;
}

PageView::ItemList PageView::getItems() const
{
    ItemList res;
    Eina_List *list = elm_box_children_get(m_Body);
    Eina_List *l = nullptr;
    void *data = nullptr;

    EINA_LIST_FOREACH(list, l, data)
    {
        PageViewItem *item = static_cast<PageViewItem*>(getSmartData((Evas_Object*)data));
        if(item)
        {
            res.push_back(item);
        }
    }

    eina_list_free(list);
    return res;
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
    m_pBox = elm_box_add(parent);
    evas_object_show(m_pBox);
    return m_pBox;
}

