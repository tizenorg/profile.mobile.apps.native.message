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

#include "SmilAttachmentItemView.h"
#include "Resource.h"

using namespace Msg;

namespace
{
    const char *attachmentGroup = "smil_attachment_item";
    const char * attachmentLayoutGroup = "smil_attachment_item_layout";
    const char *iconPart = "item.thumbnail.icon";
    const char *fileNamePart = "item.filename.text";
    const char *fileSizePart = "item.filesize.text";
    const char *attachmentLayoutContentPart = "swallow.content";
}

SmilAttachmentItemView::SmilAttachmentItemView(Evas_Object *parent, const std::string &thumbFilePath)
    : m_pItem(nullptr)
    , m_pListener(nullptr)
{
    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, attachmentLayoutGroup));
    setContent(createItem(getEo(), thumbFilePath), attachmentLayoutContentPart);
}

SmilAttachmentItemView::~SmilAttachmentItemView()
{

}

void SmilAttachmentItemView::setListener(ISmilAttachmentItemViewListener *l)
{
    m_pListener = l;
}

void SmilAttachmentItemView::setFileName(const std::string &name)
{
    elm_object_part_text_set(m_pItem, fileNamePart, name.c_str());
}

void SmilAttachmentItemView::setFileSize(const std::string &size)
{
    elm_object_part_text_set(m_pItem, fileSizePart, size.c_str());
}

void SmilAttachmentItemView::setFilePath(const std::string &filePath)
{
    m_FilePath = filePath;
}

const std::string &SmilAttachmentItemView::getFilePath() const
{
    return m_FilePath;
}

Evas_Object *SmilAttachmentItemView::createItem(Evas_Object *parent, const std::string &thumbFilePath)
{
    m_pItem = addLayout(parent, SMIL_ITEM_EDJ_PATH, attachmentGroup);
    elm_object_signal_callback_add(m_pItem, "clicked", "*", EDJE_SIGNAL_CALLBACK(SmilAttachmentItemView, onClickedCb), this);

    Evas_Object *icon = createIcon(m_pItem, thumbFilePath);
    elm_object_part_content_set(m_pItem, iconPart, icon);

    evas_object_show(m_pItem);
    return m_pItem;
}

Evas_Object *SmilAttachmentItemView::createIcon(Evas_Object *parent, const std::string &thumbFilePath)
{
    Evas_Object *thumbnail = elm_icon_add(parent);
    if(thumbFilePath.empty())
    {
        std::string imageEdjePath = PathUtils::getResourcePath(IMAGES_EDJ_PATH);
        elm_image_file_set(thumbnail, imageEdjePath.c_str(), ATTACH_IMG);
    }
    else
    {
        // TODO: impl. if needed
    }
    evas_object_size_hint_aspect_set(thumbnail, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
    evas_object_color_set(thumbnail, ICON_COLOR);
    evas_object_show(thumbnail);
    return thumbnail;
}

void SmilAttachmentItemView::onClickedCb(Evas_Object *obj, const char *emission, const char *source)
{
    if(m_pListener)
        m_pListener->onItemClicked(*this);
}

// SmilAttachmentInfoItemView:

SmilAttachmentInfoItemView::SmilAttachmentInfoItemView(Evas_Object *parent, bool manyAttachments)
{
    setEo(elm_entry_add(parent));
    elm_entry_editable_set(getEo(), false);

    if(manyAttachments)
        setText(msgt("IDS_MSGF_POP_HELP_PAGE_MESSAGE"));
    else
        setText(msgt("IDS_MSGF_POP_HELP_PAGE_MESSAGE_FOR_ONE_FILE"));
}

SmilAttachmentInfoItemView::~SmilAttachmentInfoItemView()
{
}
