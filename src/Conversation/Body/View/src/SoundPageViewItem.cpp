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

#include "SoundPageViewItem.h"
#include "FileUtils.h"

using namespace Msg;

namespace
{
    const char *labelLayout = "conv/body/media_label";
    const char *fileNamePart = "text.filename";
}

SoundPageViewItem::SoundPageViewItem(PageView &parent, const std::string &resourcePath, const std::string &dispName)
    : MediaPageViewItem(parent, resourcePath)
{
    std::string fileName = dispName.empty() ? FileUtils::getFileName(resourcePath) : dispName;
    Evas_Object *label = createLabel(getButtonLayout(), fileName);
    setButtonContent(label);
}

SoundPageViewItem::~SoundPageViewItem()
{

}

SoundPageViewItem::Type SoundPageViewItem::getType() const
{
    return SoundType;
}

bool SoundPageViewItem::isEmpty() const
{
    return false;
}

void SoundPageViewItem::highlight(bool value)
{
}

Evas_Object *SoundPageViewItem::createLabel(Evas_Object *parent, const std::string &fileName)
{
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, getEdjPath().c_str(), labelLayout);
    elm_object_part_text_set(layout, fileNamePart, fileName.c_str());
    evas_object_show(layout);
    return layout;
}

