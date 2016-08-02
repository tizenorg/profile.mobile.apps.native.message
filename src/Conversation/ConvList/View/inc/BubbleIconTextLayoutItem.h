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

#ifndef BubbleIconTextLayoutItem_h_
#define BubbleIconTextLayoutItem_h_

#include "BubbleBgViewItem.h"

namespace Msg
{
    class BubbleIconTextLayoutItem
        : public BubbleBgViewItem
    {
        public:
            enum LayoutType
            {
                Layout1Icon1Text,
                Layout1Icon2Text
            };
            static const int iconSize = 50;

        public:
            BubbleIconTextLayoutItem(BubbleEntity &entity, Evas_Object *parent, BgType bgType, LayoutType layoutType);
            virtual ~BubbleIconTextLayoutItem();

            LayoutType getLayoutType() const;
            void setIcon(Evas_Object *icon);
            void setMainText(const std::string &text);
            void setSubText(const std::string &text);

            static Evas_Object *createIcon(Evas_Object *parent, const std::string &edjFileName, BgType bgType);
            Evas_Object *createIcon(const std::string &edjFileName) const;

        private:
            LayoutType m_LayoutType;
            Evas_Object *m_pIconTextLayout;
    };
}

#endif /* BubbleIconTextLayoutItem_h_ */
