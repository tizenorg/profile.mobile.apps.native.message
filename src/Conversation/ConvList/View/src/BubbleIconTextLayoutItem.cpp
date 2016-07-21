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

#include "BubbleIconTextLayoutItem.h"
#include "Resource.h"
#include "TextDecorator.h"
#include "Resource.h"

using namespace Msg;

#define DEF_ICON_COLOR  128, 128, 128, 255

BubbleIconTextLayoutItem::BubbleIconTextLayoutItem(BubbleEntity &entity, Evas_Object *parent, LayoutType layoutType)
    : BubbleViewItem(entity)
    , m_LayoutType(layoutType)
{
    const char *group = nullptr;
    switch(layoutType)
    {
        case Layout1Icon1Text:
            group = "conv/list/1icon_1text_attachment";
            break;

        case Layout1Icon2Text:
            group = "conv/list/1icon_2text_attachment";
            break;
    }

    Evas_Object *layout = addLayout(parent, CONV_LIST_ATTACHMENTS_EDJ_PATH, group);
    evas_object_size_hint_align_set(layout, 0.0, EVAS_HINT_FILL);
    setEo(layout);

}

BubbleIconTextLayoutItem::~BubbleIconTextLayoutItem()
{
}

void BubbleIconTextLayoutItem::setIcon(Evas_Object *icon)
{
    setContent(icon, "icon");
}

void BubbleIconTextLayoutItem::setMainText(const std::string &text)
{
    setText(text, "main.text");
}

void BubbleIconTextLayoutItem::setSubText(const std::string &text)
{
    setText(text, "sub.text");
}

Evas_Object *BubbleIconTextLayoutItem::createIcon(Evas_Object *parent, const std::string &edjFileName)
{
    Evas_Object *icon = elm_icon_add(parent);
    std::string resPath = PathUtils::getResourcePath(IMAGES_EDJ_PATH);
    elm_image_file_set(icon, resPath.c_str(), edjFileName.c_str());
    evas_object_color_set(icon, DEF_ICON_COLOR);
    evas_object_show(icon);
    return icon;
}

BubbleIconTextLayoutItem::LayoutType BubbleIconTextLayoutItem::getLayoutType() const
{
    return m_LayoutType;
}
