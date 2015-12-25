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

#ifndef ConvListLayout_h_
#define ConvListLayout_h_

#include "View.h"

namespace Msg
{
    class ConvListLayout
        : public View
    {
        public:
            ConvListLayout(Evas_Object *parent);
            virtual ~ConvListLayout();

        protected:
            void showSelectAllMode(bool value);
            void setBubbleList(Evas_Object *layout);
            void setSelectAll(Evas_Object *layout);

        private:
            void create(Evas_Object *parent);
    };
}

#endif /* ConvListLayout_h_ */
