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

#ifndef SoundPageViewItem_h_
#define SoundPageViewItem_h_

#include "MediaPageViewItem.h"

namespace Msg
{
    class SoundPageViewItem
        : public MediaPageViewItem
    {
        public:
            SoundPageViewItem(PageView &parent, const std::string &reourcePath);
            virtual ~SoundPageViewItem();

            virtual Type getType() const;
            virtual bool isEmpty() const;
            virtual void highlight(bool value);

        private:
            Evas_Object *createLabel(Evas_Object *parent, const std::string &fileName);
    };
}

#endif /* SoundPageViewItem_h_ */
