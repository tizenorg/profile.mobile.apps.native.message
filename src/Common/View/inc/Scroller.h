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

#ifndef Scroller_h_
#define Scroller_h_

#include "View.h"
#include <Elementary.h>

namespace Msg
{
    class Scroller
        : public View
    {
        public:
            Scroller(Evas_Object *parent);
            virtual ~Scroller();

            inline void setPpolicy(Elm_Scroller_Policy h, Elm_Scroller_Policy v);
            void navigateToBottom();
    };

    inline void Scroller::setPpolicy(Elm_Scroller_Policy h, Elm_Scroller_Policy v)
    {
        elm_scroller_policy_set(getEo(), h, v);
    }
}

#endif /* Scroller_h_ */
