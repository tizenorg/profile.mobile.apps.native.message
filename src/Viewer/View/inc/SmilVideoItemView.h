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

#ifndef SmilVideoItemView_h_
#define SmilVideoItemView_h_

#include "View.h"

namespace Msg
{
    class SmilVideoItemView
        : public View
    {
        public:
            SmilVideoItemView(Evas_Object *parent, int videoWidth, int videoHeight);
            virtual ~SmilVideoItemView();

            Evas_Object *getVideoSink() const;

        private:
            Evas_Object *createImage(Evas_Object *parent, int width, int height);
            Evas_Object *videoRectAdd(Evas_Object *parent, int width, int height);

        private:
            Evas_Object *m_pVideoSink;
    };
}

#endif // SmilVideoItemView_h_
