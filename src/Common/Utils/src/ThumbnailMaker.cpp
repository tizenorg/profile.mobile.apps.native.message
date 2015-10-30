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

#include "ThumbnailMaker.h"
#include <Elementary.h>
#include <stdlib.h>

using namespace Msg;

const int defaultThumbSize = 74;
#define COLOR_BLUE 2, 61, 132, 255
#define RAND (rand() % 220)
#define COLOR_RAND RAND, RAND, RAND, 255

Evas_Object *ThumbnailMaker::make(Evas_Object *parent, Type type, const char *path)
{
    Evas_Object *ic = elm_image_add(parent);

    if (type == UserType)
    {
       // TODO:
      //  void * mask = ea_image_effect_mask(path, MSG_THREAD_LIST_THUMB_USER_MASK_74x74, defaultThumbSize, defaultThumbSize, 0, 0);
        Evas_Object *ic_obj = elm_image_object_get(ic);
        evas_object_image_colorspace_set(ic_obj, EVAS_COLORSPACE_ARGB8888);
        evas_object_image_size_set(ic_obj, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
     //   evas_object_image_data_set(ic_obj, mask);
        evas_object_image_alpha_set(ic_obj, EINA_TRUE);
      //  evas_object_event_callback_add(ic_obj, EVAS_CALLBACK_DEL, __msg_common_image_mask_delete_cb, nullptr);
    }
    else
    {
        elm_image_file_set(ic, path, nullptr);
        evas_object_color_set(ic, COLOR_RAND);
        evas_object_size_hint_min_set(ic, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    }

    evas_object_show(ic);

    return ic;
}

