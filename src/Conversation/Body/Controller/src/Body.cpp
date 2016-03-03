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
#include "BodyMediaType.h"
#include "MsgEngine.h"
#include "Logger.h"
#include "MediaUtils.h"
#include "PopupList.h"
#include "PopupAttachmentListItem.h"

#include <assert.h>
#include <string.h>

using namespace Msg;

namespace
{
    const int defaultPageDuration = 5; // sec

    inline TextPageViewItem *getTextItem(const PageView &page)
    {
        return page ? static_cast<TextPageViewItem*>(page.getItem(PageViewItem::TextType)) : nullptr;
    }
}

Body::Body(Evas_Object *parent, App &app)
    : BodyView(parent)
    , m_pListener(nullptr)
    , m_App(app)
    , m_pOnChangedIdler(nullptr)
{
}

Body::~Body()
{
    if(m_pOnChangedIdler)
    {
        ecore_idler_del(m_pOnChangedIdler);
        m_pOnChangedIdler = nullptr;
    }
}

void Body::setListener(IBodyListener *listener)
{
    m_pListener = listener;
}

bool Body::addMedia(const std::list<std::string> &fileList)
{
    bool res = true;
    for(auto &file : fileList)
        res &= addMedia(file);
    return res;
}

bool Body::addMedia(const std::string &filePath)
{
    MSG_LOG("Try add resource:", filePath);

    if(!FileUtils::isExists(filePath) || FileUtils::isDir(filePath))
    {
        MSG_LOG_ERROR("File not exists: ", filePath);
        return false;
    }

    std::string newFilePath = m_WorkingDir.addFile(filePath);
    if(newFilePath.empty())
        return false;

    PageViewItem::Type type = getMediaType(filePath).type;
    MSG_LOG("Media type: ", type);

    PageView *page = nullptr;
    if(type != PageViewItem::UnknownType)
    {
        page = BodyView::getPageForMedia(type);
        if(!page)
            return false;

        switch(type)
        {
            case PageViewItem::ImageType:
            {
                BodyView::addImage(*page, newFilePath);
                break;
            }

            case PageViewItem::SoundType:
            {
                BodyView::addSound(*page, newFilePath);
                break;
            }

            case PageViewItem::VideoType:
            {
                addVideo(*page, newFilePath);
                break;
            }

            default:
                assert(false);
                return false;
                break;
        }
    }
    else
    {
        BodyView::addAttachment(newFilePath);
        page = &getDefaultPage();
    }

    if(page)
        BodyView::setFocus(*page, true); // TODO: check for multi insertion

    return true;
}

bool Body::isMms() const
{
    auto pages = getPages();

    if(pages.size() > 1)
        return true;

    for(PageView *page : pages)
    {
        if(isMms(*page))
            return true;
    }

    return getAttachments().size() > 0;
}

bool Body::isMms(const PageView &page) const
{
    auto pageItems = page.getItems();
    for(PageViewItem *pageItem : pageItems)
    {
        PageViewItem::Type itemType = pageItem->getType();
        if(itemType != PageViewItem::TextType)
            return true;
    }

    return false;
}

BodySmsSize Body::getSmsSize() const
{
    BodySmsSize size = {};
    TextPageViewItem *textItem = getTextItem(getDefaultPage());

    if(textItem)
    {
        std::string text = textItem->getPlainUtf8Text();

        int textLen = m_App.getMsgEngine().calculateTextLen(text);
        int maxChar = m_App.getMsgEngine().getSettings().getMessageTextMaxChar();

        int txtLenTmp = textLen > 0 ? textLen - 1 : textLen;
        size.smsCount = txtLenTmp / maxChar + 1;
        size.charsLeft = maxChar - (textLen % (maxChar + 1));
    }

    return size;
}

long long Body::getMmsSize() const
{
    return 0;
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
        PageView &pageView = i == 0 ? getDefaultPage() : *addPage();
        writePage(pageList[i], pageView);
    }

    // Attachments:
    writeAttachments(msg);
}

void Body::writePage(const MsgPage &msgPage, PageView &pageView)
{
    auto &mediaList = msgPage.getMediaList();
    for(int i = 0; i < mediaList.getLength(); ++i)
    {
        const MsgMedia &msgMedia = mediaList[i];
        switch(msgMedia.getType())
        {
            case MsgMedia::SmilText:
                writeText(msgMedia, pageView);
                break;
            case MsgMedia::SmilImage:
                writeImage(msgMedia, pageView);
                break;
            case MsgMedia::SmilAudio:
                writeSound(msgMedia, pageView);
                break;
            case MsgMedia::SmilVideo:
                writeVideo(msgMedia, pageView);
                break;

            default:
                MSG_LOG_WARN("Skip unsupported Smil type");
                break;
        }
    }
}

void Body::writeText(const MsgMedia &msgMedia, PageView &pageView)
{
    TextPageViewItem *textItem = getTextItem(pageView);
    assert(textItem);
    if(textItem)
        textItem->setText(FileUtils::readTextFile(msgMedia.getFilePath()));
}

void Body::writeImage(const MsgMedia &msgMedia, PageView &pageView)
{
    std::string path = m_WorkingDir.addFile(msgMedia.getFilePath());
    if(!path.empty())
        addImage(pageView, path);
}

void Body::writeVideo(const MsgMedia &msgMedia, PageView &pageView)
{
    std::string path = m_WorkingDir.addFile(msgMedia.getFilePath());
    if(!path.empty())
        addVideo(pageView, path);
}

void Body::writeSound(const MsgMedia &msgMedia, PageView &pageView)
{
    std::string path = m_WorkingDir.addFile(msgMedia.getFilePath());
    if(!path.empty())
        addSound(pageView, path, msgMedia.getFileName());
}

void Body::writeAttachments(const MessageMms &msg)
{
    auto &attachmentList = msg.getAttachmentList();
    for(int i = 0; i < attachmentList.getLength(); ++i)
    {
        const MsgAttachment &attachment = attachmentList[i];
        std::string newFilePath = m_WorkingDir.addFile(attachment.getFilePath());
        if(!newFilePath.empty())
            addAttachment(newFilePath, attachment.getFileName());
    }
}

void Body::writeTextToFile(TextPageViewItem &item)
{
    if(item.getResourcePath().empty())
        item.setResourcePath(m_WorkingDir.addTextFile(item.getPlainUtf8Text()));
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
    auto pages = BodyView::getPages();
    for(PageView *page : pages)
    {
        MsgPage &msgPage = msg.addPage();

        readText(msgPage, *page);
        readImage(msgPage, *page);
        readVideo(msgPage, *page);
        readSound(msgPage, *page);

        if(msgPage.getPageDuration() < defaultPageDuration)
            msgPage.setPageDuration(defaultPageDuration);
    }

    // Attachments:
    readAttachments(msg);
}

void Body::readText(MsgPage &msgPage, const PageView &pageView)
{
    TextPageViewItem *textItem = static_cast<TextPageViewItem*>(pageView.getItem(PageViewItem::TextType));
    if(textItem)
    {
        writeTextToFile(*textItem);
        MsgMedia &media = msgPage.addMedia();
        media.setType(MsgMedia::SmilText);
        media.setFilePath(textItem->getResourcePath());
    }
    else
    {
        MSG_ASSERT(false, "TextPageViewItem is null");
    }
}

void Body::readSound(MsgPage &msgPage, const PageView &pageView)
{
    SoundPageViewItem *soundItem = static_cast<SoundPageViewItem*>(pageView.getItem(PageViewItem::SoundType));
    if(soundItem)
    {
        MsgMedia &media = msgPage.addMedia();
        media.setType(MsgMedia::SmilAudio);
        media.setFilePath(soundItem->getResourcePath());
        int sec = MediaUtils::getDurationSec(soundItem->getResourcePath());
        if(msgPage.getPageDuration() < sec)
            msgPage.setPageDuration(sec);
    }
}

void Body::readImage(MsgPage &msgPage, const PageView &pageView)
{
    ImagePageViewItem *imgItem = static_cast<ImagePageViewItem*>(pageView.getItem(PageViewItem::ImageType));
    if(imgItem)
    {
        MsgMedia &media = msgPage.addMedia();
        media.setType(MsgMedia::SmilImage);
        media.setFilePath(imgItem->getResourcePath());
    }
}

void Body::readVideo(MsgPage &msgPage, const PageView &pageView)
{
    VideoPageViewItem *videoItem = static_cast<VideoPageViewItem*>(pageView.getItem(PageViewItem::VideoType));
    if(videoItem)
    {
        MsgMedia &media = msgPage.addMedia();
        media.setType(MsgMedia::SmilVideo);
        media.setFilePath(videoItem->getResourcePath());
        int sec = MediaUtils::getDurationSec(videoItem->getResourcePath());
        if(msgPage.getPageDuration() < sec)
            msgPage.setPageDuration(sec);
    }
}

void Body::readAttachments(MessageMms &msg)
{
    auto attachments = getAttachments();
    for(BodyAttachmentView *attachView : attachments)
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

    //TODO: implement fill of subject.

    addMedia(cmd->getFileList());
}


bool Body::addVideo(PageView &page, const std::string &videoFilePath)
{
    const std::string thumbFileName = "thumbnail.jpeg";
    std::string thumbFilePath =  m_WorkingDir.genUniqueFilePath(thumbFileName);

    // FIXME: if getVideoFrame returns false ?
    MediaUtils::getVideoFrame(videoFilePath, thumbFilePath);
    BodyView::addVideo(page, videoFilePath, thumbFilePath);

    return true;
}

void Body::onItemDelete(PageViewItem &item)
{
    MSG_LOG("");
    if(auto video = dynamic_cast<VideoPageViewItem*>(&item))
        m_WorkingDir.removeFile(video->getImagePath());

    m_WorkingDir.removeFile(item.getResourcePath());
}

void Body::onItemDelete(BodyAttachmentView &item)
{
    m_WorkingDir.removeFile(item.getResourcePath());
}

void Body::onClicked(MediaPageViewItem &item)
{
    MSG_LOG("");
    PopupList &popupList = m_App.getPopupManager().getPopupList();
    if (!popupList.isVisible())
    {
        popupList.setAutoDismissBlockClickedFlag(true);
        popupList.appendItem(*new PopupAttachmentListItem(popupList, msg("IDS_MSGF_OPT_REMOVE"), item,
                POPUPLIST_ITEM_PRESSED_CB(Body, onRemoveItemPressed), this));
        popupList.show();
    }
}

void Body::onRemoveItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    PopupAttachmentListItem &listItem = static_cast<PopupAttachmentListItem&>(item);
    PageViewItem &pageViewItem = listItem.getPageItem();
    pageViewItem.destroy();
    item.getParent().destroy();
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
