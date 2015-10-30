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

#include "PageView.h"

#include <vector>

namespace Msg
{
    typedef std::vector<PageView*> PageViewCollection;
    typedef std::vector<BodyViewItem*> BodyViewItemCollection;

    class BodyView
        : public View
    {
        friend class PageView;

        public:
            BodyView(Evas_Object *parent);
            virtual ~BodyView();

            void insertAfter(BodyViewItem &item, BodyViewItem &after);
            void insertBefore(BodyViewItem &item, BodyViewItem &before);
            void append(BodyViewItem &item);
            void prepend(BodyViewItem &item);
            void remove(BodyViewItem &item);
            PageViewCollection getPages() const;
            BodyViewItemCollection getAllItems() const;
            PageView *getFocusedPage() const;
            bool getFocus() const;

        private:
            // Page event:
            virtual void onCursorChanged(PageView &page) {};
            virtual void onFocused(PageView &page) {};
            virtual void onUnfocused(PageView &page) {};
            virtual void onChanged(PageView &page) {};
            virtual void onPreeditChanged(PageView &page) {};
            virtual void onPress(PageView &page) {};
            virtual void onClicked(PageView &page) {};
            virtual void onMaxLengthReached(PageView &page) {};
            virtual void onKeyDown(PageView &page) {};
            virtual void onKeyUp(PageView &page) {};

        private:
            void create(Evas_Object *parent);
            void prepare(BodyViewItem &item);

        private:
            BodyViewItemCollection m_Items;
    };
}

#endif /* BodyView_h_ */
