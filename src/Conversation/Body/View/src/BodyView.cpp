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
#include "PageView.h"
#include "BodyAttachmentView.h"
#include "PageSeparator.h"

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

void BodyView::addPage(PageView &page)
{
    int pageCount = getItemCount(BodyViewItem::PageType);
    if(pageCount > 0)
    {
        PageSeparator *sep = new PageSeparator(*this);
        sep->setText(std::to_string(pageCount) + '/' + m_MaxPageLabel);
        append(*sep);
    }
    append(page);
}

void BodyView::removePage(PageView &page)
{
    // TODO: impl
}

void BodyView::setMaxPageLabel(const std::string &max)
{
    m_MaxPageLabel = max;
}

int BodyView::getItemCount(BodyViewItem::Type type) const
{
    int count = 0;
    auto list = getAllItems();
    for(BodyViewItem *item : list)
    {
        if(item->getType() == type)
        {
            ++count;
        }
    }
    return count;
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
    return getItems<PageView>();
}

BodyAttachmentCollection BodyView::getAttachments() const
{
    return getItems<BodyAttachmentView>();
}

template<typename T>
std::vector<T*> BodyView::getItems() const
{
    std::vector<T*> res;
    BodyViewItemCollection all = getAllItems();
    for(BodyViewItem *item : all)
    {
        if(T *itemT = dynamic_cast<T*>(item))
        {
            res.push_back(itemT);
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


