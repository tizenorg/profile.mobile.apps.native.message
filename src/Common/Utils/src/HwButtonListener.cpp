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

#include "HwButtonListener.h"

#include <cstddef>
#include <cassert>

using namespace Msg;

static void back_event_cb(void *data, Evas_Object *obj, void *event_info)
{
    static_cast<IHwButtonListener*>(data)->onHwBackButtonClicked();
}

static void more_event_cb(void *data, Evas_Object *obj, void *event_info)
{
    static_cast<IHwButtonListener*>(data)->onHwMoreButtonClicked();
}

void IHwButtonListener::setHwButtonListener(Evas_Object * obj, IHwButtonListener *listener)
{
    if(listener)
    {
        eext_object_event_callback_add(obj, EEXT_CALLBACK_MORE, more_event_cb, listener);
        eext_object_event_callback_add(obj, EEXT_CALLBACK_BACK, back_event_cb, listener);
    }
    else
    {
        eext_object_event_callback_del(obj, EEXT_CALLBACK_MORE, more_event_cb);
        eext_object_event_callback_del(obj, EEXT_CALLBACK_BACK, back_event_cb);
    }
}

