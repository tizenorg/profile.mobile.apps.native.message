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

#ifndef SmilTextItemView_h_
#define SmilTextItemView_h_

#include "View.h"

namespace Msg
{
    class SmilTextItemView
        : public View
    {
        public:
            SmilTextItemView(Evas_Object *parent, const std::string &text);
            virtual ~SmilTextItemView();

        private:
            Evas_Object *createEntry(Evas_Object *parent, const std::string &text);
    };
}

#endif // SmilTextItemView_h_
