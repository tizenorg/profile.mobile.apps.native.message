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

#ifndef PageView_h_
#define PageView_h_

#include "BodyViewItem.h"
#include "PageViewItem.h"

#include <vector>
#include <map>

namespace Msg
{
    class BodyView;
    class PageView
        : public BodyViewItem
    {
        public:
            typedef std::vector<PageViewItem*> ItemList;

        public:
            PageView(BodyView &parent);
            virtual ~PageView();

            ItemList getItems() const;
            void addItem(PageViewItem &item);
            void removeItem(PageViewItem &item);
            PageViewItem *getItem(PageViewItem::Type type) const;
            bool isEmpty() const;
            Evas_Object *getItemParent() const;

        private:
            typedef std::map<PageViewItem::Type, PageViewItem*> PageItemMap;

            Evas_Object *createMainLayout(Evas_Object *parent);
            Evas_Object *createLayout(Evas_Object *parent);
            Evas_Object *createBox(Evas_Object *parent);

        private:
            Evas_Object *m_pBox;
            PageItemMap m_PageItemMap;
    };
}

#endif /* PageView_h_ */
