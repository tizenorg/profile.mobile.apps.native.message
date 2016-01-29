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

#include "BubbleView.h"
#include "FileUtils.h"

#include <Elementary.h>

using namespace Msg;

namespace
{
    const int verticalBoxPads = 10;
    const int horizontalBoxPads = 0;
    const int maxWidth = 220; //Fixme: set to 340 when apply base_scale: 2.6
    const char *textStyle = "DEFAULT='font=Tizen:style=Regular font_size=30 wrap=mixed text_class=label'";
}

BubbleView::BubbleView(Evas_Object *parent)
{
    create(parent);
}

BubbleView::~BubbleView()
{

}

void BubbleView::create(Evas_Object *parent)
{
    Evas_Object *box = elm_box_add(parent);
    View::expand(box);
    elm_box_padding_set(box, (int)ELM_SCALE_SIZE(horizontalBoxPads), (int)ELM_SCALE_SIZE(verticalBoxPads));

    setEo(box);
}

void BubbleView::fill(const BubbleEntity &entity)
{
    for(const BubbleEntity::BubblePart &part : entity.m_Parts)
    {
        switch (part.type)
        {
            case BubbleEntity::TextPart:
                elm_box_pack_end(*this, createTextView(part.value));
                break;
            case BubbleEntity::TextFilePart:
                elm_box_pack_end(*this, createTextFileView(part.value));
                break;
            case BubbleEntity::ThumbnailPart:
                elm_box_pack_end(*this, createThumbView(part.value));
                break;
            default:
                break;
        }
    }
}

Evas_Object *BubbleView::createTextView(const std::string &text)
{
    //TODO: apply to label the same text style as to textblock (figure out how-to)
    Evas_Coord ww, hh;
    Evas_Object *label = elm_label_add(*this);
    Evas_Object *textBlock = evas_object_textblock_add(evas_object_evas_get(label));
    Evas_Textblock_Style *ts = evas_textblock_style_new();

    evas_textblock_style_set(ts, textStyle);
    evas_object_textblock_style_set(textBlock, ts);
    evas_object_textblock_text_markup_set(textBlock, text.c_str());
    evas_object_textblock_size_native_get(textBlock, &ww, &hh);

    evas_textblock_style_free(ts);
    evas_object_del(textBlock);

    int wrapWidth = ww > maxWidth ? (int)ELM_SCALE_SIZE(maxWidth) : (int)ELM_SCALE_SIZE(ww);
    elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    elm_label_wrap_width_set(label, wrapWidth);
    elm_object_part_text_set(label, nullptr, text.c_str());
    evas_object_show(label);
    evas_object_size_hint_align_set(label, 0.0, EVAS_HINT_FILL);
    return label;
}

Evas_Object *BubbleView::createTextFileView(const std::string &path)
{
    std::string text = FileUtils::readTextFile(path);
    if(text.empty())
        return nullptr;
    else
        return createTextView(text);
}

Evas_Object *BubbleView::createThumbView(const std::string &path)
{
    Evas_Object *image = elm_image_add(*this);
    elm_image_aspect_fixed_set(image, false);
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

BubbleEntity::BubbleEntity()
{
}

BubbleEntity::~BubbleEntity()
{
}

void BubbleEntity::addPart(PartType type, const std::string &value)
{
    m_Parts.push_back(BubblePart{type, value});
}
