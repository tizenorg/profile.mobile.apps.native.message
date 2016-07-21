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

#include "BubbleImageViewItem.h"

using namespace Msg;

BubbleImageViewItem::BubbleImageViewItem(BubbleEntity &entity, Evas_Object *parent, const std::string &imagePath)
    : BubbleViewItem(entity)
{
    Evas_Object *img = createImage(parent, imagePath);
    attachGestureTapLayer(img, img);
    setEo(img);
}

BubbleImageViewItem::~BubbleImageViewItem()
{
}

Evas_Object *BubbleImageViewItem::createImage(Evas_Object *parent, const std::string &path)
{
    Evas_Object *image = elm_image_add(parent);
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
    return image;
}
