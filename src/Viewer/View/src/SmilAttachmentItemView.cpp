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
#include "FileUtils.h"
#include "LangUtils.h"
#include "Logger.h"

using namespace Msg;

namespace
{
    const float fileButtonWeight = 2.0 / 3.0;
    const float saveButtonWeight = 1.0 / 3.0;
    const char *layoutGroupName = "smil_attachment_item_layout";
    const char *layoutSwallowContent = "swallow.content";
    #define paddingBetweenButton ELM_SCALE_SIZE(16)
}

SmilAttachmentItemView::SmilAttachmentItemView(Evas_Object *parent)
    : m_pListener(nullptr)
    , m_pFileButton(nullptr)
    , m_pSaveButton(nullptr)
{
    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, layoutGroupName));

    // Box:
    Evas_Object *box = elm_box_add(getEo());
    elm_box_horizontal_set(box, true);
    elm_box_homogeneous_set(box, false);
    elm_box_padding_set(box, paddingBetweenButton, 0);
    evas_object_show(box);

    setContent(box, layoutSwallowContent);

    // File button:
    m_pFileButton = elm_button_add(box);
    evas_object_size_hint_weight_set(m_pFileButton, fileButtonWeight, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_pFileButton, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_smart_callback_add
    (
        m_pFileButton,
        "clicked",
        [] (void *data, Evas_Object *obj, void *event_info)
        {
            auto *self = (SmilAttachmentItemView*)data;
            if(self->m_pListener)
                self->m_pListener->onItemClicked(*self);
        },
        this
    );
    evas_object_show(m_pFileButton);

    // Save button:
    m_pSaveButton = elm_button_add(box);
    evas_object_size_hint_weight_set(m_pSaveButton, saveButtonWeight, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(m_pSaveButton, EVAS_HINT_FILL, EVAS_HINT_FILL);
    setText(m_pSaveButton, msgt("IDS_MSG_BUTTON_SAVE_ABB2"));
    evas_object_smart_callback_add
    (
        m_pSaveButton,
        "clicked",
        [] (void *data, Evas_Object *obj, void *event_info)
        {
            auto *self = (SmilAttachmentItemView*)data;
            if(self->m_pListener)
                self->m_pListener->onSaveButtonClicked(*self);
        },
        this
    );
    evas_object_show(m_pSaveButton);

    elm_box_pack_end(box, m_pFileButton);
    elm_box_pack_end(box, m_pSaveButton);
}

SmilAttachmentItemView::~SmilAttachmentItemView()
{

}

void SmilAttachmentItemView::setListener(ISmilAttachmentItemViewListener *l)
{
    m_pListener = l;
}

const std::string &SmilAttachmentItemView::getFilePath() const
{
    return m_FilePath;
}

void SmilAttachmentItemView::setFileName(const std::string &name)
{
    elm_object_text_set(m_pFileButton, name.c_str());
}

void SmilAttachmentItemView::setFilePath(const std::string &filePath)
{
    m_FilePath = filePath;
}

// SmilAttachmentInfoItemView:
SmilAttachmentInfoItemView::SmilAttachmentInfoItemView(Evas_Object *parent, bool manyAttachments)
{
    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, layoutGroupName));

    Evas_Object *entry = elm_entry_add(getEo());
    elm_entry_editable_set(entry, false);
    evas_object_show(entry);

    setContent(entry, layoutSwallowContent);

    if(manyAttachments)
        setText(entry, msgt("IDS_MSGF_POP_HELP_PAGE_MESSAGE"));
    else
        setText(entry, msgt("IDS_MSGF_POP_HELP_PAGE_MESSAGE_FOR_ONE_FILE"));
}

SmilAttachmentInfoItemView::~SmilAttachmentInfoItemView()
{
}

// SmilSaveAllItemView:
SmilSaveAllItemView::SmilSaveAllItemView(Evas_Object *parent, int count)
    : m_pButton(nullptr)
    , m_pListener(nullptr)
    , m_Count(count)
{
    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, layoutGroupName));
    m_pButton = elm_button_add(getEo());
    updateTitle();
    evas_object_smart_callback_add(m_pButton, "language,changed", SMART_CALLBACK(SmilSaveAllItemView, onLanguageChanged), this);
    evas_object_smart_callback_add
    (
        m_pButton,
        "clicked",
        [] (void *data, Evas_Object *obj, void *event_info)
        {
            auto *self = (SmilSaveAllItemView*)data;
            if(self->m_pListener)
                self->m_pListener->onItemClicked(*self);
        },
        this
    );
    evas_object_show(m_pButton);
    setContent(m_pButton, layoutSwallowContent);
}

SmilSaveAllItemView::~SmilSaveAllItemView()
{

}

void SmilSaveAllItemView::setListener(ISmilSaveAllItemViewListener *l)
{
    m_pListener = l;
}

void SmilSaveAllItemView::updateTitle()
{
    std::string title = msg("IDS_MSGF_BUTTON_SAVE_ALL_ATTACHMENTS_ABB");
    title += " (" + std::to_string(m_Count) + ")";
    elm_object_text_set(m_pButton, title.c_str());
}

void SmilSaveAllItemView::onLanguageChanged(Evas_Object *obj, void *eventInfo)
{
    MSG_LOG("");
    updateTitle();
}
