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

#ifndef BubbleTextViewItem_h_
#define BubbleTextViewItem_h_

#include "BubbleBgViewItem.h"

namespace Msg
{
    class BubbleTextViewItem
        : public BubbleBgViewItem
    {
        public:
            BubbleTextViewItem(BubbleEntity &entity, Evas_Object *parent, BgType bgType, const std::string &text);
            virtual ~BubbleTextViewItem();

        private:
            Evas_Object *createText(Evas_Object *parent, const std::string &text);
    };
}

#endif /* BubbleTextViewItem_h_ */
