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

#include "BubbleViewItem.h"

namespace Msg
{
    class BubbleIconTextLayoutItem
        : public BubbleViewItem
    {
        public:
            enum LayoutType
            {
                Layout1Icon1Text,
                Layout1Icon2Text
            };

        public:
            BubbleIconTextLayoutItem(BubbleEntity &entity, Evas_Object *parent, LayoutType layoutType);
            virtual ~BubbleIconTextLayoutItem();

            void setIcon(Evas_Object *icon);
            void setMainText(const std::string &text);
            void setSubText(const std::string &text);

            static Evas_Object *createIcon(Evas_Object *parent, const std::string &edjFileName);
    };
}

#endif /* BubbleIconTextLayoutItem_h_ */
