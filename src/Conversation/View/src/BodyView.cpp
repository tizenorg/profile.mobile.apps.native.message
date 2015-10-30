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

#include "BodyView.h"
#include <assert.h>

using namespace Msg;

namespace
{
    const char *objKey = "obj";
}

BodyView::BodyView(Evas_Object *parent)
{
    create(parent);
}

BodyView::~BodyView()
{

}

void BodyView::create(Evas_Object *parent)
{
    setEo(elm_box_add(parent));
    expand();
}

void BodyView::prepare(BodyViewItem &item)
{
    item.setData(objKey, &item);
}

void BodyView::append(BodyViewItem &item)
{
    prepare(item);
    elm_box_pack_end(getEo(), item);
    item.show();
}

void BodyView::prepend(BodyViewItem &item)
{
    prepare(item);
    elm_box_pack_start(getEo(), item);
    item.show();
}

void BodyView::insertAfter(BodyViewItem &item, BodyViewItem &after)
{
    prepare(item);
    elm_box_pack_after(getEo(), item, after);
}

void BodyView::insertBefore(BodyViewItem &item, BodyViewItem &before)
{
    prepare(item);
    elm_box_pack_before(getEo(), item, before);
}

void BodyView::remove(BodyViewItem &item)
{
    elm_box_unpack(getEo(), item);
    item.destroy();
}

PageViewCollection BodyView::getPages() const
{
    PageViewCollection res;
    BodyViewItemCollection all = getAllItems();
    for(BodyViewItem *item : all)
    {
        if(item->getType() == BodyViewItem::PageType)
        {
            res.push_back(static_cast<PageView*>(item));
        }
    }

    return res;
}

BodyViewItemCollection BodyView::getAllItems() const
{
    BodyViewItemCollection res;
    Eina_List *list = elm_box_children_get(getEo());
    if(list)
    {
        Eina_List *l = nullptr;
        void *obj = nullptr;

        EINA_LIST_FOREACH(list, l, obj)
        {
            BodyViewItem *item = static_cast<BodyViewItem*>(evas_object_data_get((Evas_Object*)obj, objKey));
            assert(item);
            if(item)
                res.push_back(item);
        }
        eina_list_free(list);
    }

    return res;
}

PageView *BodyView::getFocusedPage() const
{
    PageView *focusedPage = nullptr;
    PageViewCollection pages = getPages();

    for(PageView *page : pages)
    {
        if(page->getFocus())
        {
            focusedPage = page;
            break;
        }
    }
    return focusedPage;
}

bool BodyView::getFocus() const
{
    return getFocusedPage() != nullptr;
}

