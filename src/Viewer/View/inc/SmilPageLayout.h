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

#ifndef SmilPageLayout_h_
#define SmilPageLayout_h_

#include "View.h"

namespace Msg
{
    class SmilPageLayout
        : public View
    {
        public:
            SmilPageLayout(Evas_Object *parent);
            virtual ~SmilPageLayout();

            void appendItem(Evas_Object *item);

        private:
            Evas_Object *createScroller(Evas_Object *parent);
            Evas_Object *createBox(Evas_Object *parent);

        private:
            Evas_Object *m_pBox;
    };
}

#endif // SmilPageLayout_h_
