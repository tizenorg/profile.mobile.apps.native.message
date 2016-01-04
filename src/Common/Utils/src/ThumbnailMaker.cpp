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
#include "PathUtils.h"
#include "Logger.h"

#include <Elementary.h>
#include <stdlib.h>

using namespace Msg;

const int defaultThumbSize = 74;
#define COLOR_BLUE 2, 61, 132, 255
#define RAND (rand() % 220)
#define COLOR_RAND RAND, RAND, RAND, 255
#define MSG_THUMB_STYLE_LIST    "list_ic_user_thumb_masking"

Evas_Object *ThumbnailMaker::make(Evas_Object *parent, Type type, const std::string &path)
{
    Evas_Object *ic = nullptr;

    if (type == UserType)
    {
        ic = elm_layout_add(parent);
        std::string edjePath = PathUtils::getResourcePath(THUMBNAIL_EDJ_PATH);
        elm_layout_file_set(ic, edjePath.c_str(), MSG_THUMB_STYLE_LIST);

        Evas_Object *img = elm_image_add(ic);
        elm_image_file_set(img, path.c_str(), nullptr);
        evas_object_size_hint_min_set(img, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
        elm_image_fill_outside_set(img, EINA_TRUE);
        elm_object_part_content_set(ic, "content", img);
    }
    else
    {
        ic = elm_image_add(parent);
        elm_image_file_set(ic, path.c_str(), nullptr);
        evas_object_color_set(ic, COLOR_RAND);
        evas_object_size_hint_min_set(ic, ELM_SCALE_SIZE(defaultThumbSize), ELM_SCALE_SIZE(defaultThumbSize));
    }

    evas_object_show(ic);

    return ic;
}

