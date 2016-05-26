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

#include "App.h"
#include "Body.h"
#include "PageView.h"
#include "FileUtils.h"
#include "TextPageViewItem.h"
#include "ImagePageViewItem.h"
#include "VideoPageViewItem.h"
#include "MediaType.h"
#include "MsgEngine.h"
#include "Logger.h"
#include "MediaUtils.h"
#include "PopupList.h"
#include "PopupBodyAttachmentListItem.h"

#include <assert.h>
#include <string.h>
#include <notification_status.h>

using namespace Msg;

namespace
{
    const std::string contactFileName = "Contact.vcf";
    const std::string contactsFileName = "Contacts.vcf";

    inline TextPageViewItem *getTextItem(const PageView &page)
    {
        return page ? static_cast<TextPageViewItem*>(page.getItem(PageViewItem::TextType)) : nullptr;
    }

    PageViewItem::Type msgMediaTypeToPageItemType(MsgMedia::Type type)
    {
        switch(type)
        {
            case MsgMedia::ImageType:
                return PageViewItem::ImageType;
            case MsgMedia::AudioType:
                return PageViewItem::SoundType;
            case MsgMedia::VideoType:
                return PageViewItem::VideoType;
            case MsgMedia::TextType:
                return PageViewItem::TextType;
            default:
                return PageViewItem::UnknownType;
        }
    }
}

Body::Body(App &app, WorkingDirRef workingDir)
    : BodyView()
    , m_pListener(nullptr)
    , m_App(app)
    , m_WorkingDir(workingDir)
    , m_pOnChangedIdler(nullptr)
    , m_TooLargePopupShow(false)
    , m_TooMuchAttachedPopupShow(false)
    , m_ResizingPopupShow(false)
    , m_MmsRecipFlag(false)
    , m_AttachmentHandler(workingDir)
{
    m_AttachmentHandler.setListener(this);
}

void Body::create(Evas_Object *parent)
{
    BodyView::create(parent);

}

Page &Body::getDefaultPage()
{
    return static_cast<Page&>(BodyView::getDefaultPage());
}

Body::~Body()
{
    if(m_pOnChangedIdler)
    {
        ecore_idler_del(m_pOnChangedIdler);
        m_pOnChangedIdler = nullptr;
    }
}

Page &Body::createPage()
{
    Page *page = new Page(*this, m_WorkingDir);
    BodyView::addText(*page, m_App.getMsgEngine().getSettings().getMaxMsgTextLen());
    return *page;
}

void Body::setListener(IBodyListener *listener)
{
    m_pListener = listener;
}

void Body::addMedia(const std::list<std::string> &fileList)
{
    for(auto& file : fileList)
        m_SelectedFiles.push(file);
    runFileProcessing();
}

void Body::addMedia(const std::string &filePath)
{
    m_SelectedFiles.push(filePath);
    runFileProcessing();
}

void Body::runFileProcessing()
{
    if(!m_SelectedFiles.empty())
    {
        long long freeSpace = m_App.getMsgEngine().getSettings().getMaxMmsSize() - getMsgSize();
        if(freeSpace < FileUtils::getFileSize(m_SelectedFiles.front()))
            showResizingPopup();
        m_AttachmentHandler.processFile(m_SelectedFiles.front());
    }
}

bool Body::isMms()
{
    if(m_MmsRecipFlag)
        return true;

    auto pages = getPages();

    if(pages.size() > 1)
        return true;

    if(getAttachments().size() > 0)
        return true;

    for(PageView *page : pages)
    {
        if(static_cast<Page*>(page)->isMms())
            return true;
    }

    return false;
}

void Body::setMmsRecipFlag(bool value)
{
    m_MmsRecipFlag = value;
}

const MsgTextMetric &Body::getTextMetric()
{
    return static_cast<Page&>(getDefaultPage()).getTextMetric();
}

long long Body::getMsgSize()
{
    // Attachments:
    long long size = 0;
    auto attachments = getAttachments();
    for(BodyAttachmentViewItem *attachment : attachments)
    {
        long long fileSize = attachment->getFileSize();
        if(fileSize > 0)
            size += fileSize;
    }

    // Pages:
    auto pages = getPages();
    for(PageView *pageView : pages)
    {
        size += static_cast<Page*>(pageView)->getSize();
    }

    return size;
}

int Body::getAttachmentsCountTotal() const
{
    int count = getAttachments().size();

    auto pages = getPages();
    for(PageView *pageView : pages)
        count += static_cast<Page*>(pageView)->getAttachmentsCount();

    return count;
}

void Body::read(Message &msg)
{
    MSG_LOG("");
    if(MessageSMS *sms = dynamic_cast<MessageSMS*>(&msg))
        read(*sms);
    else if(MessageMms *mms = dynamic_cast<MessageMms*>(&msg))
        read(*mms);
}

void Body::write(const Message &msg)
{
    if(const MessageSMS *sms = dynamic_cast<const MessageSMS*>(&msg))
        write(*sms);
    else if(const MessageMms *mms = dynamic_cast<const MessageMms*>(&msg))
        write(*mms);
}

void Body::write(const MessageSMS &msg)
{
    TextPageViewItem *textItem = getTextItem(getDefaultPage());
    assert(textItem);
    if(textItem)
        textItem->setText(msg.getText());
}

void Body::write(const MessageMms &msg)
{
    // Pages:
    auto &pageList = msg.getPageList();
    for(int i = 0; i < pageList.getLength(); ++i)
    {
        Page &page = i == 0 ? getDefaultPage() : static_cast<Page&>(*addPage());
        page.write(pageList[i]);
    }

    // Attachments:
    writeAttachments(msg);
}

void Body::writeAttachments(const MessageMms &msg)
{
    auto &attachmentList = msg.getAttachmentList();
    for(int i = 0; i < attachmentList.getLength(); ++i)
    {
        const MsgAttachment &attachment = attachmentList[i];
        addAttachment(attachment.getFilePath(), attachment.getFileName());
    }
}

void Body::writeTextToFile(TextPageViewItem &item)
{
    if(item.getResourcePath().empty())
        item.setResourcePath(m_WorkingDir->addTextFile(item.getPlainUtf8Text()));
    else
        FileUtils::writeTextFile(item.getResourcePath(), item.getPlainUtf8Text());
}

void Body::read(MessageSMS &msg)
{
    TextPageViewItem *textItem = getTextItem(getDefaultPage());
    assert(textItem);
    if(textItem)
        msg.setText(textItem->getPlainUtf8Text());
}

void Body::read(MessageMms &msg)
{
    // Pages:
    auto pages = getPages();
    for(PageView *page : pages)
    {
        MsgPage &msgPage = msg.addPage();
        static_cast<Page*>(page)->read(msgPage);
    }

    // Attachments:
    readAttachments(msg);
}

void Body::readAttachments(MessageMms &msg)
{
    auto attachments = getAttachments();
    for(BodyAttachmentViewItem *attachView : attachments)
    {
        std::string resPath= attachView->getResourcePath();
        std::string mime = getMediaType(resPath).mime;
        long long fileSize = FileUtils::getFileSize(resPath);

        MsgAttachment &msgAttach = msg.addAttachment();
        msgAttach.setFilePath(resPath);
        msgAttach.setFileName(attachView->getFileName());
        msgAttach.setFileSize((int)fileSize);
        msgAttach.setMime(mime);
    }
}

void Body::execCmd(const AppControlComposeRef &cmd)
{
    TextPageViewItem *textItem = getTextItem(getDefaultPage());
    if(textItem)
        textItem->setText(cmd->getMessageText());

    std::list<std::string> path = cmd->getFileList();
    if(!cmd->getVcfInfo().contactsIdList.empty())
        path.push_back(createVcfFile(cmd));

    //TODO: implement fill of subject.

    addMedia(path);
}

void Body::addAttachment(const std::string &filePath, const std::string &fileName)
{
    std::string newFilePath = m_WorkingDir->addFile(filePath);
    if(!newFilePath.empty())
    {
        long long fileSize = FileUtils::getFileSize(newFilePath);
        std::string newFileName = fileName.empty() ? FileUtils::getFileName(filePath) : fileName;
        BodyView::addAttachment(newFilePath, fileSize, newFileName);
    }
}

void Body::onItemDelete(PageViewItem &item)
{
    MSG_LOG("");
    if(auto video = dynamic_cast<VideoPageViewItem*>(&item))
        m_WorkingDir->removeFile(video->getImagePath());

    m_WorkingDir->removeFile(item.getResourcePath());
}

void Body::onItemDelete(BodyAttachmentViewItem &item)
{
    m_WorkingDir->removeFile(item.getResourcePath());
}

void Body::onMaxLengthReached(TextPageViewItem &item)
{
    MSG_LOG("");
    showMaxCharactersPopup();
}

void Body::onCheckBoundaryText(TextPageViewItem &item, char **text)
{
    MSG_LOG("");
    if(isMms())
    {
        long long maxSize = m_App.getMsgEngine().getSettings().getMaxMmsSize();
        std::string utfText = markupToUtf8(*text);
        if(getMsgSize() + static_cast<long long>(utfText.size()) > maxSize)
        {
            free(*text);
            *text = nullptr;
            showMaxCharactersPopup();
        }
    }
}

void Body::onClicked(MediaPageViewItem &item)
{
    MSG_LOG("");
    PopupList &popupList = createPopupList(item.getFileName());
    popupList.appendItem(*new PopupBodyAttachmentListItem(popupList, msg("IDS_MSGF_OPT_REMOVE"), item,
                POPUPLIST_ITEM_PRESSED_CB(Body, onRemoveMediaItemPressed), this));
    popupList.show();
}

void Body::onClicked(BodyAttachmentViewItem &item)
{
    MSG_LOG("");
    PopupList &popupList = createPopupList(item.getFileName());

    popupList.appendItem(*new BodyAttachmentListItem(popupList, msg("IDS_MSGF_OPT_REMOVE"), item,
                POPUPLIST_ITEM_PRESSED_CB(Body, onRemoveBodyItemPressed), this));
    popupList.show();
}

PopupList &Body::createPopupList(const std::string &title)
{
    PopupList &popupList = m_App.getPopupManager().getPopupList();
    popupList.setTitle(title);
    popupList.setAutoDismissBlockClickedFlag(true);
    return popupList;
}

void Body::showTooLargePopup(const std::list <std::string> &tooBigFiles)
{
   if(!m_TooLargePopupShow)
   {
        Popup &popup = m_App.getPopupManager().getPopup();
        popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Body, onTooLargePopupDel), this);
        popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId);
        popup.setTitle(msgt("IDS_MSG_HEADER_FILE_SIZE_TOO_LARGE_ABB"));
        std::string content(msg("IDS_MSG_POP_UNABLE_TO_ATTACH_FILE_FILE_SIZE_TOO_LARGE_TRY_SENDING_VIA_EMAIL_BLUETOOTH_WI_FI_ETC"));
        for(auto& file: tooBigFiles)
        {
            content.append("<br/>");
            content.append(FileUtils::getFileName(file));
        }
        popup.setContent(content);
        popup.setAutoDismissBlockClickedFlag(true);
        popup.show();
        m_TooLargePopupShow = true;
   }
}

void Body::showTooMuchAttachedPopup(int willBeAttached)
{
   if(!m_TooMuchAttachedPopupShow)
   {
        Popup &popup = m_App.getPopupManager().getPopup();
        popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Body, onTooMuchAttachedPopupDel), this);
        popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId);
        int maxCount = m_App.getMsgEngine().getSettings().getAttachmentsMaxCount();
        std::string content(msgArgs("IDS_MSGF_BODY_MAXIMUM_NUMBER_OF_ATTACHMENTS_HP1SS_EXCEEDED_ONLY_FIRST_P2SS_WILL_BE_ADDED", std::to_string(maxCount).c_str(), std::to_string(willBeAttached).c_str()));
        popup.setContent(content);
        popup.setAutoDismissBlockClickedFlag(true);
        popup.show();
        m_TooMuchAttachedPopupShow = true;
   }
}

void Body::showTooMuchAttachedPopup()
{
   if(!m_TooMuchAttachedPopupShow)
   {
        Popup &popup = m_App.getPopupManager().getPopup();
        popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Body, onTooMuchAttachedPopupDel), this);
        popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId);
        int maxCount = m_App.getMsgEngine().getSettings().getAttachmentsMaxCount();
        std::string content(msgArgs("IDS_MSGF_POP_MAXIMUM_NUMBER_OF_ATTACHMENTS_HPS_EXCEEDED", std::to_string(maxCount).c_str()));
        popup.setContent(content);
        popup.setAutoDismissBlockClickedFlag(true);
        popup.show();
        m_TooMuchAttachedPopupShow = true;
   }
}

void Body::showResizingPopup()
{
    if(!m_ResizingPopupShow)
    {
        Popup &popup = m_App.getPopupManager().getPopup();
        popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Body, onResizingPopupDel), this);
        int maxSize = m_App.getMsgEngine().getSettings().getMaxMmsSize();
        std::string content(msgArgs("IDS_MSG_TPOP_MAXIMUM_MESSAGE_SIZE_HPS_EXCEEDED_RESIZING_ATTACHMENTS_ING", std::to_string(maxSize).c_str()));
        popup.setContent(content);
        popup.setAutoDismissBlockClickedFlag(true);
        popup.show();
        m_ResizingPopupShow = true;
    }
}

void Body::hideResizingPopup()
{
    m_App.getPopupManager().reset();
}

void Body::showMaxCharactersPopup()
{
    MSG_LOG("");
    std::string notifText = msg("IDS_MSGF_POP_MAXIMUM_CHARACTERS");
    notification_status_message_post(notifText.c_str());
}

void Body::onRemoveMediaItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    PopupBodyAttachmentListItem &listItem = static_cast<PopupBodyAttachmentListItem&>(item);
    PageViewItem &pageViewItem = listItem.getPageItem();
    pageViewItem.destroy();
    item.getParent().destroy();
}

void Body::onRemoveBodyItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    BodyAttachmentListItem &listItem = static_cast<BodyAttachmentListItem&>(item);
    BodyAttachmentViewItem &bodyItem = listItem.getBodyItem();
    bodyItem.destroy();
    item.getParent().destroy();
}

void Body::onTooLargePopupDel(Evas_Object *obj, void *eventInfo)
{
    MSG_LOG("");
    m_TooLargePopupShow = false;
}

void Body::onTooMuchAttachedPopupDel(Evas_Object *obj, void *eventInfo)
{
    MSG_LOG("");
    m_TooMuchAttachedPopupShow = false;
}

void Body::onResizingPopupDel(Evas_Object *obj, void *eventInfo)
{
    MSG_LOG("");
    m_ResizingPopupShow = false;
}

std::string Body::createVcfFile(const AppControlComposeRef &cmd)
{
    auto &idList = cmd->getVcfInfo().contactsIdList;
    std::string content;
    std::string path;

    if(cmd->getComposeType() == AppControlCompose::OpShare)
    {
        content = m_App.getContactManager().makeVcard(*idList.begin(), cmd->getVcfInfo().isMyProfile);
        path = contactFileName;
    }
    else if(cmd->getComposeType() == AppControlCompose::OpMultiShare)
    {
        content = m_App.getContactManager().makeVcard(idList);
        path = contactsFileName;
    }

    path = content.empty() ? std::string() : m_WorkingDir->addTextFile(content, path);

    return path;
}

void Body::onContentChanged()
{
    if(!m_pOnChangedIdler)
    {
        m_pOnChangedIdler = ecore_idler_add
        (
            [](void *data)->Eina_Bool
            {
                Body *self =(Body*)data;
                if(self->m_pListener)
                    self->m_pListener->onChanged(*self);
                self->m_pOnChangedIdler = nullptr;
                return false; // Delete idler
            },
            this
        );
    }
}

void Body::onFileReady(const std::string &filePath)
{
    int numAttached = getAttachmentsCountTotal();
    int numAttachMax = m_App.getMsgEngine().getSettings().getAttachmentsMaxCount();
    if(numAttached == numAttachMax)
    {
        std::queue <std::string> empty;
        std::swap(m_SelectedFiles, empty);
        showTooMuchAttachedPopup();
        return;
    }
    MediaTypeData mediaType = getMediaType(filePath);
    MSG_LOG("Media type: ", mediaType.mime);

    Page *page = nullptr;
    if(mediaType.type != MsgMedia::UnknownType &&
            mediaType.type != MsgMedia::TextType)
    {
        page = static_cast<Page*>(getPageForMedia(msgMediaTypeToPageItemType(mediaType.type)));
        if(!page)
            return;

        page->addMedia(filePath);

    }
    else
    {
        addAttachment(filePath);
        page = &getDefaultPage();
    }

    if(page)
        BodyView::setFocus(*page, true); // TODO: check for multi insertion
    m_SelectedFiles.pop();
    if(!m_SelectedFiles.empty())
    {
        long long freeSpace = m_App.getMsgEngine().getSettings().getMaxMmsSize() - getMsgSize();
        if(freeSpace < FileUtils::getFileSize(m_SelectedFiles.front()))
            showResizingPopup();
        m_AttachmentHandler.processFile(m_SelectedFiles.front());
    }
    else
    {
        hideResizingPopup();
    }
}

void Body::onFileFails()
{
    std::list <std::string> overflowList;
    while (!m_SelectedFiles.empty())
    {
        overflowList.push_back(m_SelectedFiles.front());
        m_SelectedFiles.pop();
    }
    showTooLargePopup(overflowList);
}

long long Body::onFreeSpaceRequest()
{
    return m_App.getMsgEngine().getSettings().getMaxMmsSize() - getMsgSize();
}
