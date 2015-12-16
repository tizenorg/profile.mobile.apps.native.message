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

#ifndef BodyView_h_
#define BodyView_h_

#include "View.h"
#include "BodyViewItem.h"

#include <vector>

namespace Msg
{
    class PageView;
    class BodyAttachmentView;

    typedef std::vector<PageView*> PageViewCollection;
    typedef std::vector<BodyAttachmentView*> BodyAttachmentCollection;
    typedef std::vector<BodyViewItem*> BodyViewItemCollection;

    class BodyView
        : public View
    {
        friend class PageView;

        public:
            BodyView(Evas_Object *parent);
            virtual ~BodyView();

            void setMaxPageLabel(const std::string &max);
            void addPage(PageView &page);
            void removePage(PageView &page);
            PageViewCollection getPages() const;
            BodyAttachmentCollection getAttachments() const;
            template<typename T>
            std::vector<T*> getItems() const;
            BodyViewItemCollection getAllItems() const;
            PageView *getFocusedPage() const;
            bool getFocus() const;
            int getItemCount(BodyViewItem::Type type) const;

        private:
            void create(Evas_Object *parent);
            void prepare(BodyViewItem &item);
            void insertAfter(BodyViewItem &item, BodyViewItem &after);
            void insertBefore(BodyViewItem &item, BodyViewItem &before);
            void append(BodyViewItem &item);
            void prepend(BodyViewItem &item);
            void remove(BodyViewItem &item);

        private:
            BodyViewItemCollection m_Items;
            std::string m_MaxPageLabel;
    };
}

#endif /* BodyView_h_ */
