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

#ifndef BubbleBgViewItem_h_
#define BubbleBgViewItem_h_

#include "BubbleViewItem.h"

namespace Msg
{
    class BubbleBgViewItem
        : public BubbleViewItem
    {
        public:
            enum BgType
            {
                SentStyle,
                ReceivedStyle,
                DraftStyle,
                FailedStyle
            };

        public:
            BubbleBgViewItem(BubbleEntity &entity, Evas_Object *parent, BgType bgType);
            virtual ~BubbleBgViewItem();

            void showSearch(bool search);
            void setContent(Evas_Object *obj);
            Evas_Object *getContent() const;
    };
}

#endif /* BubbleBgViewItem_h_ */
