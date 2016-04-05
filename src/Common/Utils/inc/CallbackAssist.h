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

#ifndef CallBackAssist_h_
#define CallBackAssist_h_

#include <Evas.h>
#include <app.h>

#define SMART_CALLBACK(ClassName, method) [](void *data, Evas_Object *obj, void *event_info) \
{                                                                                            \
    static_cast<ClassName*>(data)->method(obj, event_info);                                  \
}

#define EVAS_EVENT_CALLBACK(ClassName, method) [](void *data, Evas *e, Evas_Object *obj, void *event_info) \
{                                                                                                          \
    static_cast<ClassName*>(data)->method(obj, event_info);                                                \
}

#define MBE_ITEM_FILTER_CALLBACK(ClassName, method) [](Evas_Object *obj, const char *item_label, const void *item_data, const void *data)->Eina_Bool \
{                                                                                                                                                    \
    return static_cast<ClassName*>((data*)data)->method(obj, item_label, item_data);                                                                 \
}

#define ECORE_TACK_CALLBACK(ClassName, method) [](void *data)->Eina_Bool \
{                                                                        \
    return static_cast<ClassName*>(data)->method();                      \
}

#define APP_EVENT_CALLBACK(ClassName, method) [](app_event_info_h event_info, void *data)\
{                                                                        \
    return static_cast<ClassName*>(data)->method(event_info);                      \
}

#define EDJE_SIGNAL_CALLBACK(ClassName, method) [](void *data, Evas_Object *obj, const char *emission, const char *source) \
{                                                                                                                          \
    static_cast<ClassName*>(data)->method(obj, emission, source);                                                          \
}

#endif /* CallBackAssist_h_ */
