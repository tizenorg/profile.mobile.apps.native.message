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

#ifndef HwButtonListener_h_
#define HwButtonListener_h_

#include <Evas.h>
#include <efl_extension.h>

namespace Msg
{
    class IHwButtonListener
    {
        public:
            virtual ~IHwButtonListener() {};
            virtual void onHwBackButtonClicked() {};
            virtual void onHwMoreButtonClicked() {};

            static void setHwButtonListener(Evas_Object *obj, IHwButtonListener *listener);
    };
}

#endif /* HwButtonListener_h_ */
