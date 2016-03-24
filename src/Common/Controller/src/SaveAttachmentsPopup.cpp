/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "PopupAttachmentListItem.h"
#include "SaveAttachmentsPopup.h"
#include <string.h>
#include "PopupManager.h"
#include "MsgMedia.h"
#include "FileUtils.h"
#include <media_content.h>
#include <media_content_type.h>
#include <notification_status.h>

using namespace Msg;

SaveAttachmentsPopup::SaveAttachmentsPopup(App &app, const MessageMms &mms)
    : PopupList(app.getPopupManager())
    , m_App(app)
    , m_SaveButton(nullptr)
{
    create(mms);
}
SaveAttachmentsPopup::~SaveAttachmentsPopup()
{
}

void SaveAttachmentsPopup::create(const MessageMms &mms)
{
    setTitle(msgt("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"));
    addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB2"), Popup::CancelButtonId, POPUP_BUTTON_CB(SaveAttachmentsPopup, onCancelButtonClicked), this);
    m_SaveButton = addButton(msgt("IDS_MSG_BUTTON_SAVE_ABB2"), Popup::OkButtonId, POPUP_BUTTON_CB(SaveAttachmentsPopup, onSaveButtonClicked), this);
    disableSaveButton(true);

    getListView().setMode(ELM_LIST_COMPRESS);
    getListView().setCheckMode(true);
    getListView().setScrollerContentMinLimit(EINA_FALSE, EINA_TRUE);

    fillList(mms);
    if(getListView().getItems().size() > 1)
        showSelectAllItem();

    getListView().checkAllItems(false);
}

void SaveAttachmentsPopup::showSelectAllItem()
{
    SelectAllListItem *item = dynamic_cast<SelectAllListItem*>(getListView().getFirstItem());
    if(!item)
    {
        item = new SelectAllListItem;
        getListView().prependItem(*item);
    }
}

bool SaveAttachmentsPopup::areAllItemsChecked() const
{
    ListItemCollection items = getListView().getItems();

    auto item = items.empty() ? items.begin() : ++items.begin();
    for(; item != items.end(); ++item)
    {
        if((*item)->isCheckable() && !(*item)->getCheckedState())
            return false;
    }
    return true;
}

void SaveAttachmentsPopup::updateSelectAllItem()
{
    if(getListView().getItems().size() > 1)
    {
        bool allChecked = areAllItemsChecked();

        SelectAllListItem *selectAllItem = dynamic_cast<SelectAllListItem*>(getListView().getFirstItem());
        if(selectAllItem)
            selectAllItem->setCheckedState(allChecked, true);
    }
}

void SaveAttachmentsPopup::fillList(const MessageMms &mms)
{
    const MsgPageList &pageList = mms.getPageList();
    for(int i = 0; i < pageList.getLength(); ++i)
    {
        const MsgMediaList &mediaList = pageList.at(i).getMediaList();
        for(int j = 0; j < mediaList.getLength(); ++j)
        {
            MsgMedia::Type type = mediaList.at(j).getType();
            if(type != MsgMedia::TextType && type != MsgMedia::UnknownType)
            {
                appendItem(mediaList.at(j).getFileName(), mediaList.at(j).getFilePath(), nullptr, this);
            }
        }
    }

    const MsgAttachmentList &attachmentList = mms.getAttachmentList();
    for(int i = 0; i < attachmentList.getLength(); ++i)
    {
        appendItem(attachmentList.at(i).getFileName(), attachmentList.at(i).getFilePath(), nullptr, this);
    }
}

void SaveAttachmentsPopup::onCancelButtonClicked(Popup &popup, int buttonId)
{
    popup.destroy();
}

void SaveAttachmentsPopup::onSaveButtonClicked(Popup &popup, int buttonId)
{
    MSG_LOG("");
    bool result = false;
    if(media_content_connect() != 0)
    {
        popup.destroy();
        showSavingFailedPopup();
        return;
    }

    result = saveCheckedItems();
    popup.destroy();

    if(result)
        notification_status_message_post(msg("IDS_MSGF_POP_SAVED_IN_MY_FILES").cStr());
    else
        showSavingFailedPopup();

    media_content_disconnect();
}

bool SaveAttachmentsPopup::isSaveButtonNeedToBeEnable()
{
    ListItemCollection items = getListView().getItems();
    for(auto *item : items)
    {
        if(item->isCheckable() && item->getCheckedState())
            return true;
    }
    return false;
}

void SaveAttachmentsPopup::disableSaveButton(bool value)
{
    elm_object_disabled_set(m_SaveButton, value);
}

bool SaveAttachmentsPopup::saveCheckedItems()
{
    bool success = true;
    std::string filePathDst;
    std::string dowloadPath = PathUtils::getDownloadPath();

    auto items = getListView().getItems<PopupAttachmentListItem>();
    for(auto *item : items)
    {
        if(item->isCheckable() && item->getCheckedState())
        {
            filePathDst = FileUtils::genUniqueFilePath(dowloadPath, item->getFilePath());
            bool copySucceed = FileUtils::copy(item->getFilePath(), filePathDst);
            success &= copySucceed && (media_content_scan_file(filePathDst.c_str()) == MEDIA_CONTENT_ERROR_NONE);
        }
    }

    return success;
}

void SaveAttachmentsPopup::showSavingFailedPopup()
{
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.setContent(msgt("IDS_MSGF_POP_SAVING_FAILED"));
    popup.setTimeOut(1.0);
    popup.show();
}

void SaveAttachmentsPopup::onListItemChecked(ListItem &listItem)
{
    if(SelectAllListItem *it = dynamic_cast<SelectAllListItem*>(&listItem))
        checkSelectAllItem(*it);
    else
        checkItem(listItem);
}

void SaveAttachmentsPopup::checkSelectAllItem(SelectAllListItem &item)
{
    bool checked = item.getCheckedState();
    getListView().checkAllItems(checked);
    disableSaveButton(!checked);
}

void SaveAttachmentsPopup::checkItem(ListItem &item)
{
    updateSelectAllItem();
    disableSaveButton(!isSaveButtonNeedToBeEnable());
}
