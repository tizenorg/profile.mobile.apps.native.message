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
#include "Logger.h"

#include <efl_extension.h>
#include <assert.h>

using namespace Msg;

namespace
{
    const char *pageGroup = "conv/body/page";
    const char *itemLayoutGroup = "conv/body/item_layout";
}

PageView::PageView(BodyView &parent)
    : BodyViewItem(PageType)
    , m_pBox(nullptr)
{
    setEo(createMainLayout(parent));
    Evas_Object *layout = createLayout(getEo());
    Evas_Object *box = createBox(layout);

    elm_object_part_content_set(layout, "swl.page", box);
    setContent(layout, "swl.content");
}

PageView::~PageView()
{

}

bool PageView::isEmpty() const
{
    for(auto it : m_PageItemMap)
    {
        if(!it.second->isEmpty())
            return false;
    }
    return true;
}

Evas_Object *PageView::getItemParent() const
{
    return m_pBox;
}

bool PageView::hasMedia() const
{
    for(auto item : m_PageItemMap)
    {
        if(item.first != PageViewItem::TextType)
            return true;
    }
    return false;
}

bool PageView::canItemBeAdded(PageViewItem::Type type) const
{
    auto it = m_PageItemMap.find(type);
    if(it != m_PageItemMap.end())
        return false;

    if(type == PageViewItem::VideoType && hasMedia())
        return false;

    if(getItem(PageViewItem::VideoType) && type != PageViewItem::TextType)
        return false;

    return true;
}

void PageView::addItem(PageViewItem &item)
{
    /* Page items order:
     * Image/Video
     * Text
     * Audio
     */
    switch(item.getType())
    {
        case PageViewItem::TextType:
        {
            // Middle:
            auto itEnd = m_PageItemMap.end();
            auto it = m_PageItemMap.find(PageViewItem::ImageType);
            if(it != itEnd)
                elm_box_pack_after(m_pBox, item, *it->second);
            else if((it = m_PageItemMap.find(PageViewItem::SoundType)) != itEnd)
                elm_box_pack_before(m_pBox, item, *it->second);
            else
                elm_box_pack_start(m_pBox, item);
            break;
        }

        case PageViewItem::VideoType:
        case PageViewItem::ImageType:
        {
            // Top:
            elm_box_pack_start(m_pBox, item);
            break;
        }

        case PageViewItem::SoundType:
        {
            // Bottom:
            elm_box_pack_end(m_pBox, item);
            break;
        }

        default:
            assert(false);
            return;
            break;
    }

    item.expand();
    item.show();

    m_PageItemMap[item.getType()] = &item;
}

void PageView::removeItem(PageViewItem &item)
{
    auto it = m_PageItemMap.find(item.getType());
    if(it != m_PageItemMap.end())
        m_PageItemMap.erase(it);
    item.View::destroy();
}

PageViewItem *PageView::getItem(PageViewItem::Type type) const
{
    auto it = m_PageItemMap.find(type);
    return it == m_PageItemMap.end() ? nullptr : it->second;
}

PageView::ItemList PageView::getItems() const
{
    ItemList res;
    Eina_List *list = elm_box_children_get(m_pBox);
    Eina_List *l = nullptr;
    void *data = nullptr;

    EINA_LIST_FOREACH(list, l, data)
    {
        PageViewItem *item = reinterpretCast<PageViewItem*>(data);
        res.push_back(item);
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

Evas_Object *PageView::createMainLayout(Evas_Object *parent)
{
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, getEdjPath().c_str(), itemLayoutGroup);
    emitSignal(layout, "show.normal.mode", "*");
    expand(layout);
    evas_object_show(layout);
    return layout;
}

Evas_Object *PageView::createBox(Evas_Object *parent)
{
    m_pBox = elm_box_add(parent);
    elm_box_homogeneous_set(m_pBox, false);
    evas_object_show(m_pBox);
    return m_pBox;
}


