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

#include "Page.h"
#include "PageView.h"
#include "Body.h"
#include "Logger.h"
#include "MsgEngine.h"
#include "TextPageViewItem.h"
#include "MediaPageViewItem.h"
#include "FileUtils.h"
#include "MediaUtils.h"
#include "MediaType.h"

using namespace Msg;

namespace
{
    const int defaultPageDuration = 5; // sec
}

Page::Page(Body &parent, WorkingDirRef workingDir)
    : PageView(parent)
    , m_Body(parent)
    , m_MsgMetric()
    , m_WorkingDir(workingDir)
    , m_Utf8TextSize(0)
{
}

Page::~Page()
{
}

const MsgTextMetric &Page::getTextMetric()
{
    updateMsgMetricIfNeeded();
    return m_MsgMetric;
}

void Page::updateMsgMetricIfNeeded()
{
    TextPageViewItem *item = static_cast<TextPageViewItem*>(getItem(PageViewItem::TextType));
    if(item)
    {
        if(item->hasChanged())
        {
            std::string text = item->getPlainUtf8Text();
            m_Utf8TextSize = text.length();
            MsgEngine::calculateTextMetric(text, m_MsgMetric);
            item->resetChangedFlag();
        }
    }
    else
    {
        m_MsgMetric.reset();
    }
}

long long Page::getSize()
{
    long long totalSize = 0;
    auto items = getItems();
    for(PageViewItem *item : items)
    {
        MediaPageViewItem *mediaItem = dynamic_cast<MediaPageViewItem*>(item);
        if(mediaItem)
        {
            int size = mediaItem->getFileSize();
            if(size > 0)
                totalSize += size;
        }
    }
    updateMsgMetricIfNeeded();
    totalSize += m_Body.isMms() ? m_Utf8TextSize : m_MsgMetric.bytes;
    return totalSize;
}

int Page::getAttachmentsCount() const
{
    int res = 0;
    auto items = getItems();
    for(PageViewItem *item : items)
    {
        MediaPageViewItem *mediaItem = dynamic_cast<MediaPageViewItem*>(item);
        if(mediaItem)
            ++res;
    }
    return res;
}

bool Page::isMms()
{
    auto pageItems = getItems();
    for(PageViewItem *pageItem : pageItems)
    {
        PageViewItem::Type itemType = pageItem->getType();
        if(itemType != PageViewItem::TextType)
            return true;
    }

    updateMsgMetricIfNeeded();
    return m_MsgMetric.isMms;
}

bool Page::addMedia(const std::string &filePath)
{
    MediaTypeData typeData = getMediaType(filePath);
    MSG_LOG("Media type: ", typeData.mime);

   switch(typeData.type)
   {
       case MsgMedia::ImageType:
       {
           addImage(filePath);
           break;
       }

       case MsgMedia::AudioType:
       {
           addSound(filePath);
           break;
       }

       case MsgMedia::VideoType:
       {
           addVideo(filePath);
           break;
       }

       default:
           MSG_LOG_WARN("Can't add type: ", typeData.mime);
           return false;
           break;
   }

   return true;
}

void Page::write(const MsgPage &msgPage)
{
    auto &mediaList = msgPage.getMediaList();
    for(int i = 0; i < mediaList.getLength(); ++i)
    {
        const MsgMedia &msgMedia = mediaList[i];
        switch(msgMedia.getType())
        {
            case MsgMedia::TextType:
                writeText(msgMedia);
                break;
            case MsgMedia::ImageType:
                writeImage(msgMedia);
                break;
            case MsgMedia::AudioType:
                writeSound(msgMedia);
                break;
            case MsgMedia::VideoType:
                writeVideo(msgMedia);
                break;

            default:
                MSG_LOG_WARN("Skip unsupported Smil type");
                break;
        }
    }
}

void Page::writeText(const MsgMedia &msgMedia)
{
    TextPageViewItem *textItem = static_cast<TextPageViewItem*>(getItem(PageViewItem::TextType));;
    assert(textItem);
    if(textItem)
        textItem->setText(FileUtils::readTextFile(msgMedia.getFilePath()));
}

void Page::writeImage(const MsgMedia &msgMedia)
{
    addImage(msgMedia.getFilePath());
}

void Page::writeVideo(const MsgMedia &msgMedia)
{
    addVideo(msgMedia.getFilePath());
}

void Page::writeSound(const MsgMedia &msgMedia)
{
    addSound(msgMedia.getFilePath(), msgMedia.getFileName());
}

void Page::writeTextToFile(TextPageViewItem &item)
{
    if(item.getResourcePath().empty())
        item.setResourcePath(m_WorkingDir->addTextFile(item.getPlainUtf8Text()));
    else
        FileUtils::writeTextFile(item.getResourcePath(), item.getPlainUtf8Text());
}

void Page::read(MsgPage &msgPage)
{
    readText(msgPage);
    readImage(msgPage);
    readVideo(msgPage);
    readSound(msgPage);

    if(msgPage.getPageDuration() < defaultPageDuration)
        msgPage.setPageDuration(defaultPageDuration);
}

void Page::readText(MsgPage &msgPage)
{
    TextPageViewItem *textItem = static_cast<TextPageViewItem*>(getItem(PageViewItem::TextType));
    if(textItem)
    {
        writeTextToFile(*textItem);
        MsgMedia &media = msgPage.addMedia();
        media.setFilePath(textItem->getResourcePath());
    }
    else
    {
        MSG_ASSERT(false, "TextPageViewItem is null");
    }
}

void Page::readSound(MsgPage &msgPage)
{
    SoundPageViewItem *soundItem = static_cast<SoundPageViewItem*>(getItem(PageViewItem::SoundType));
    if(soundItem)
    {
        MsgMedia &media = msgPage.addMedia();
        media.setFilePath(soundItem->getResourcePath());
        int sec = MediaUtils::getDurationSec(soundItem->getResourcePath());
        if(msgPage.getPageDuration() < sec)
            msgPage.setPageDuration(sec);
    }
}

void Page::readImage(MsgPage &msgPage)
{
    ImagePageViewItem *imgItem = static_cast<ImagePageViewItem*>(getItem(PageViewItem::ImageType));
    if(imgItem)
    {
        MsgMedia &media = msgPage.addMedia();
        media.setFilePath(imgItem->getResourcePath());
    }
}

void Page::readVideo(MsgPage &msgPage)
{
    VideoPageViewItem *videoItem = static_cast<VideoPageViewItem*>(getItem(PageViewItem::VideoType));
    if(videoItem)
    {
        MsgMedia &media = msgPage.addMedia();
        media.setFilePath(videoItem->getResourcePath());
        int sec = MediaUtils::getDurationSec(videoItem->getResourcePath());
        if(msgPage.getPageDuration() < sec)
            msgPage.setPageDuration(sec);
    }
}

void Page::addVideo(const std::string &videoFilePath)
{
    const std::string thumbFileName = "thumbnail.jpeg";
    std::string thumbFilePath =  m_WorkingDir->genUniqueFilePath(thumbFileName);
    std::string newVideoFilePath = m_WorkingDir->addFile(videoFilePath);

    if(!newVideoFilePath.empty())
    {
        long long fileSize = FileUtils::getFileSize(newVideoFilePath);
        // FIXME: if getVideoFrame returns false ?
        MediaUtils::getVideoFrame(videoFilePath, thumbFilePath);
        m_Body.addVideo(*this, newVideoFilePath, fileSize, thumbFilePath);
    }
}

void Page::addImage(const std::string &filePath)
{
    std::string newFilePath = m_WorkingDir->addFile(filePath);
    if(!newFilePath.empty())
    {
        long long fileSize = FileUtils::getFileSize(newFilePath);
        m_Body.addImage(*this, newFilePath, fileSize);
    }
}

void Page::addSound(const std::string &filePath, const std::string &fileName)
{
    std::string newFilePath = m_WorkingDir->addFile(filePath);
    if(!newFilePath.empty())
    {
        long long fileSize = FileUtils::getFileSize(newFilePath);
        std::string newFileName = fileName.empty() ? FileUtils::getFileName(filePath) : fileName;
        m_Body.addSound(*this, newFilePath, fileSize, newFileName);
    }
}

