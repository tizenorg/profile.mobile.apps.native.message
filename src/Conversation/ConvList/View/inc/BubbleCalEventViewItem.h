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

#ifndef BubbleCalEventViewItem_h_
#define BubbleCalEventViewItem_h_

#include "BubbleIconTextLayoutItem.h"
#include "Resource.h"

namespace Msg
{
    class BubbleCalEventViewItem
        : public BubbleIconTextLayoutItem
    {
        public:
            BubbleCalEventViewItem(BubbleEntity &entity, Evas_Object *parent, LayoutType layoutType);
            virtual ~BubbleCalEventViewItem();
    };

    inline BubbleCalEventViewItem::BubbleCalEventViewItem(BubbleEntity &entity, Evas_Object *parent, LayoutType layoutType)
        : BubbleIconTextLayoutItem(entity, parent, layoutType)
    {
        attachGestureTapLayer(getEo(), getEo());
        setIcon(createIcon(getEo(), ATTACH_CAL_ICON));
    }

    inline BubbleCalEventViewItem::~BubbleCalEventViewItem()
    {
    }
}

#endif /* BubbleCalEventViewItem_h_ */
