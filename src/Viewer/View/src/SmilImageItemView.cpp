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

#include "SmilImageItemView.h"
#include "Resource.h"
#include <notification_status.h>
#include "Logger.h"
using namespace Msg;

SmilImageItemView::SmilImageItemView(Evas_Object *parent, const std::string &imagePath)
    : m_pImage(nullptr)
{
    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, "smil_image_item"));
    Evas_Object *image = createImage(getEo(), imagePath);
    setContent(image, "image.swallow.content");
}

SmilImageItemView::~SmilImageItemView()
{

}

Evas_Object *SmilImageItemView::getImage() const
{
    return m_pImage;
}

void SmilImageItemView::playAnimation(bool play)
{
    if(hasAnimation())
        elm_image_animated_play_set(m_pImage, play);
}

bool SmilImageItemView::hasAnimation() const
{
    return elm_image_animated_get(m_pImage);
}

Evas_Object *SmilImageItemView::createImage(Evas_Object *parent, const std::string &imagePath)
{
    m_pImage = elm_image_add(parent);
    if(!elm_image_file_set(m_pImage, imagePath.c_str(), nullptr))
    {
        evas_object_del(m_pImage);
        m_pImage = nullptr;
        return m_pImage;
    }

    evas_object_show(m_pImage);

    int w = 0;
    int h = 0;

    elm_image_object_size_get(m_pImage, &w, &h);

    evas_object_size_hint_min_set(m_pImage, ELM_SCALE_SIZE(w), ELM_SCALE_SIZE(h));
    evas_object_size_hint_max_set(m_pImage, ELM_SCALE_SIZE(w), ELM_SCALE_SIZE(h));

    if(elm_image_animated_available_get(m_pImage))
        elm_image_animated_set(m_pImage, true);

    return m_pImage;
}
