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

#include "ImagePageViewItem.h"
#include "PageView.h"
#include "Logger.h"

#include <assert.h>

using namespace Msg;

namespace
{
    const char *mediaImagePortraitSig = "media.image.portrait";
    const char *mediaImageLandscapeSig = "media.image.landscape";
    const char *mediaImageEqualSig = "media.image.equal";
    const char *imageLayout = "conv/body/media_image";
    const char *imageContentPart = "swl.thumbnail";

    const int thumbOriginHeight = 75;
    const int thumbLandscapeWidth = 128;
    const int thumbLandscapeHeight = 102;
    const int thumbPortraitWidth = 102;
    const int thumbPortraitHeight = 128;
    const int thumbEqual = 106;
}

ImagePageViewItem::ImagePageViewItem(PageView &parent, const std::string &reourcePath)
    : MediaPageViewItem(parent, reourcePath)
    , m_pImageLayout(nullptr)
{
    Evas_Object *imageLayout = createImageLayout(getMediaLayout());
    Evas_Object *rect = createRect(getMediaLayout());
    Evas_Object *icon = createImageIconAndSetOrient(m_pImageLayout);

    elm_object_part_content_set(m_pImageLayout, imageContentPart, icon);
    setRect(rect);
    setButtonContent(imageLayout);
}

ImagePageViewItem::~ImagePageViewItem()
{

}

ImagePageViewItem::Type ImagePageViewItem::getType() const
{
    return ImageType;
}

bool ImagePageViewItem::isEmpty() const
{
    return getResourcePath().empty();
}

void ImagePageViewItem::highlight(bool value)
{
    const char *sig = value ? "focused" : "unfocused";
    elm_object_signal_emit(m_pImageLayout, sig, "*");
}

Evas_Object *ImagePageViewItem::createImageLayout(Evas_Object *parent)
{
    m_pImageLayout = elm_layout_add(parent);
    elm_layout_file_set(m_pImageLayout, getEdjPath().c_str(), imageLayout);
    evas_object_show(m_pImageLayout);
    return m_pImageLayout;
}

Evas_Object *ImagePageViewItem::createRect(Evas_Object *parent)
{
    Evas_Object *rect = evas_object_rectangle_add(evas_object_evas_get(parent));
    evas_object_color_set(rect, 0, 0, 0, 0);
    evas_object_size_hint_min_set(rect, 0, ELM_SCALE_SIZE(thumbOriginHeight));
    evas_object_show(rect);
    return rect;
}

Evas_Object *ImagePageViewItem::createImageIconAndSetOrient(Evas_Object *parent)
{
    const std::string resourcePath = getResourcePath();

    // Get image dimension:
    Evas *evas = evas_object_evas_get(parent);
    Evas_Object *img = evas_object_image_add(evas);
    evas_object_image_load_orientation_set(img, true);
    evas_object_image_file_set(img, resourcePath.c_str(), nullptr);

    int err = evas_object_image_load_error_get(img);
    if (err != EVAS_LOAD_ERROR_NONE)
    {
        evas_object_del(img);
        MSG_LOG_ERROR("Image loading is failed: ", resourcePath);
        return nullptr;
    }

    int width = 0;
    int height = 0;
    evas_object_image_size_get(img, &width, &height);
    evas_object_del(img);

    MSG_LOG_ERROR("Image: width = ", width, " height = ", height);

    // Set orientation  of media layout and icon dimension:
    int iconWidth = 0;
    int iconHeight = 0;
    const char *sig = nullptr;

    if(width > height) // Landscape
    {
        iconWidth = (int)ELM_SCALE_SIZE(thumbLandscapeWidth);
        iconHeight = (int)ELM_SCALE_SIZE(thumbLandscapeHeight);
        sig = mediaImageLandscapeSig;
    }
    else if(height > width) // Portrait
    {
        iconWidth = (int)ELM_SCALE_SIZE(thumbPortraitWidth);
        iconHeight = (int)ELM_SCALE_SIZE(thumbPortraitHeight);
        sig = mediaImagePortraitSig;
    }
    else // Equal
    {
        iconWidth = (int)ELM_SCALE_SIZE(thumbEqual);
        iconHeight = (int)ELM_SCALE_SIZE(thumbEqual);
        sig = mediaImageEqualSig;
    }
    elm_object_signal_emit(getMediaLayout(), sig, "*");

    // Create and load icon image:
    Evas_Object *icon = elm_icon_add(parent);
    elm_image_file_set(icon, resourcePath.c_str(), NULL);
    evas_object_size_hint_min_set(icon, iconWidth, iconHeight);
    elm_image_aspect_fixed_set(icon, EINA_FALSE);
    evas_object_show(icon);

    return icon;
}
