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

#ifndef Window_h_
#define Window_h_

#include "View.h"
#include <Evas.h>

namespace Msg
{
    /**
     * @brief An interface that should be implemented by all window-classes across whole application.
     */
    class Window
        : public View
    {
        public:
            virtual ~Window() {};

            virtual Evas_Object *getWinEvasObject() const = 0;
            virtual Evas_Object *getConformEvasObject() const = 0;
            virtual Evas_Object *getHostEvasObject() const = 0;
            virtual void setContent(Evas_Object *content) = 0;
            virtual void getScreenSize(int *x, int *y, int *w, int *h) const = 0;
            virtual int getRotation() const = 0;
    };
}

#endif /* Window_h_ */
