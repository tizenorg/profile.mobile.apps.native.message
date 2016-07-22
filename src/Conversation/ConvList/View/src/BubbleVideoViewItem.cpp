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

#include "BubbleVideoViewItem.h"
#include "Resource.h"

using namespace Msg;

BubbleVideoViewItem::BubbleVideoViewItem(BubbleEntity &entity, Evas_Object *parent, const std::string &imagePath)
    : BubbleViewItem(entity)
{
    Evas_Object *layout = createLayout(parent, imagePath);
    setEo(layout);
    attachGestureTapLayer(layout, layout);
    show();
}

BubbleVideoViewItem::~BubbleVideoViewItem()
{
}

Evas_Object *BubbleVideoViewItem::createLayout(Evas_Object *parent, const std::string &path)
{
    Evas_Object *layout = addLayout(parent, CONV_LIST_ATTACHMENTS_EDJ_PATH, "conv/list/attachments/video_item");
    Evas_Object *image = elm_image_add(layout);
    elm_image_file_set(image, path.c_str(), nullptr);
    int imageWidth = 0;
    int imageHeight = 0;
    elm_image_object_size_get(image, &imageWidth, &imageHeight);
    if(imageWidth > maxWidth)
    {
        double scale = maxWidth/(double)imageWidth;
        imageWidth *= scale;
        imageHeight *= scale;
    }
    evas_object_size_hint_min_set(image, imageWidth, imageHeight);
    evas_object_show(image);

    elm_object_part_content_set(layout, "swl.thumbnail", image);
    return layout;
}
