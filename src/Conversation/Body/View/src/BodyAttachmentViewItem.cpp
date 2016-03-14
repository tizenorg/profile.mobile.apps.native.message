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

#include "BodyAttachmentViewItem.h"
#include "BodyView.h"
#include "FileUtils.h"
#include "Logger.h"

using namespace Msg;

namespace
{
    const char *mediaMainGroup = "conv/body/media_main";
    const char *mediaMainSwlContent = "swl.content";
    const char *buttonStyle = "focus";
    const char *buttonSwlContent = "elm.swallow.content";
    const char *labelLayout = "conv/body/media_label";
    const char *fileNamePart = "text.filename";
}

BodyAttachmentViewItem::BodyAttachmentViewItem(BodyView &parent, const std::string &resourePath, long long fileSize, const std::string &dispName)
    : BodyViewItem(AttachmentType)
    , m_pLayaout(nullptr)
    , m_pListener(nullptr)
    , m_ResourePath(resourePath)
    , m_FileName(dispName)
    , m_FileSize(fileSize)
{
    setEo(createLayout(parent));
    Evas_Object *button = createButton(getEo());
    Evas_Object *label = createLabel(button, m_FileName);

    elm_layout_content_set(button, buttonSwlContent, label);
    setContent(button, mediaMainSwlContent);
}

BodyAttachmentViewItem::~BodyAttachmentViewItem()
{
    if(m_pListener)
        m_pListener->onDelete(*this);
}

void BodyAttachmentViewItem::setListener(IBodyAttachmentViewListener *listener)
{
    m_pListener = listener;
}

const std::string &BodyAttachmentViewItem::getResourcePath() const
{
   return m_ResourePath;
}

const std::string &BodyAttachmentViewItem::getFileName() const
{
    return m_FileName;
}

long long BodyAttachmentViewItem::getFileSize() const
{
    return m_FileSize;
}

Evas_Object *BodyAttachmentViewItem::createLayout(Evas_Object *parent)
{
    m_pLayaout = elm_layout_add(parent);
    elm_layout_file_set(m_pLayaout, getEdjPath().c_str(), mediaMainGroup);
    expand(m_pLayaout);
    evas_object_show(m_pLayaout);
    return m_pLayaout;
}

Evas_Object *BodyAttachmentViewItem::createButton(Evas_Object *parent)
{
    Evas_Object *button = elm_button_add(parent);
    elm_object_style_set(button, buttonStyle);
    evas_object_show(button);

    evas_object_smart_callback_add(button, "clicked", [](void *data, Evas_Object *obj, void *event_info)
    {
        BodyAttachmentViewItem *self = (BodyAttachmentViewItem*)data;
        if(self && self->m_pListener)
            self->m_pListener->onClicked(*self);
    }, this);

    return button;
}

Evas_Object *BodyAttachmentViewItem::createLabel(Evas_Object *parent, const std::string &fileName)
{
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, getEdjPath().c_str(), labelLayout);
    elm_object_part_text_set(layout, fileNamePart, fileName.c_str());
    evas_object_show(layout);
    return layout;
}
