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


#include "SmilPage.h"
#include "SmilImageItemView.h"
#include "SmilTextItemView.h"
#include "SmilAudioItemView.h"
#include "SmilVideoItemView.h"
#include "Logger.h"
#include "FileUtils.h"
#include "MediaPlayer.h"
#include "MediaUtils.h"
#include <notification.h>

using namespace Msg;

const int defaultPageDuration = 5; // sec;

SmilPage::SmilPage(Evas_Object *parent, const MsgPage &page)
    : SmilPageLayout(parent)
    , m_Duration(0)
    , m_pVideoSink(nullptr)
    , m_HasAudio(false)
    , m_pImageItem(nullptr)
    , m_HasInvalidMedia(false)
{
    build(page);
}

SmilPage::SmilPage(Evas_Object *parent, const MsgAttachmentList &list)
    : SmilPageLayout(parent)
    , m_Duration(0)
    , m_pVideoSink(nullptr)
    , m_HasAudio(false)
    , m_pImageItem(nullptr)
    , m_HasInvalidMedia(false)
{
    build(list);
}

SmilPage::~SmilPage()
{
    MSG_LOG("");
}

int SmilPage::getDuration() const
{
    return m_Duration;
}

bool SmilPage::hasMedia() const
{
    return !m_MediaPath.empty();
}

bool SmilPage::hasInvalidMedia() const
{
    return m_HasInvalidMedia;
}

bool SmilPage::hasVideo() const
{
    return m_pVideoSink != nullptr;
}

bool SmilPage::hasAudio() const
{
    return m_HasAudio;
}

Evas_Object *SmilPage::getVideoSink() const
{
    return m_pVideoSink;
}

std::string SmilPage::getMediaPath() const
{
    return m_MediaPath;
}

const std::list<std::string> &SmilPage::getAttachments() const
{
    return m_Attachments;
}

bool SmilPage::hasAnimation() const
{
    return m_pImageItem && m_pImageItem->hasAnimation();
}

void SmilPage::playAnimation(bool play)
{
    if(m_pImageItem && m_pImageItem->hasAnimation())
        m_pImageItem->playAnimation(play);
}

const MsgMedia *SmilPage::getMedia(const MsgPage &page, MsgMedia::Type type) const
{
    const MsgMediaList &list = page.getMediaList();
    for(int i = 0; i < list.getLength(); ++i)
    {
        if(list[i].getType() == type)
            return &list[i];
    }
    return nullptr;
}

void SmilPage::build(const MsgPage &page)
{
    m_Duration = page.getPageDuration();

    const MsgMediaList &list = page.getMediaList();
    for(int i = 0; i < list.getLength(); ++i)
    {
        m_Attachments.push_back(list[i].getFilePath());
    }

    // TODO: image/video, text order

    const MsgMedia *video = getMedia(page, MsgMedia::VideoType);
    if(video)
        buildVideo(*video);
    else
    {
        const MsgMedia *image = getMedia(page, MsgMedia::ImageType);
        if(image)
            buildImage(*image);
    }

    const MsgMedia *text = getMedia(page, MsgMedia::TextType);
    if(text)
        buildText(*text);

    if(!video)
    {
        const MsgMedia *audio = getMedia(page, MsgMedia::AudioType);
        if(audio)
            buildAudio(*audio);
    }

    if(m_Duration <= 0)
        m_Duration = defaultPageDuration;

    MSG_LOG("Page duration: ", m_Duration);
}

void SmilPage::build(const MsgAttachmentList &list)
{
    m_Duration = defaultPageDuration;

    for(int i = 0; i < list.getLength(); ++i)
    {
        m_Attachments.push_back(list[i].getFilePath());
    }

    if(list.isEmpty())
        return;

    buildAttachmentInfo(list.getLength());
    buildSaveAllItem(list.getLength());
    for(int i = 0; i < list.getLength(); ++i)
    {
        buildAttachment(list[i]);
    }
}

void SmilPage::buildImage(const MsgMedia &media)
{
    m_pImageItem = new SmilImageItemView(getBox(), media.getFilePath());
    m_HasInvalidMedia = m_pImageItem->getImage() == nullptr;
    m_pImageItem->show();
    appendItem(*m_pImageItem);
}

void SmilPage::buildText(const MsgMedia& media)
{
    std::string text = FileUtils::readTextFile(media.getFilePath());
    if(!text.empty())
    {
        SmilTextItemView *item = new SmilTextItemView(getBox(), text);
        item->show();
        appendItem(*item);
    }
}

void SmilPage::buildAudio(const MsgMedia& media)
{
    m_MediaPath = media.getFilePath();
    m_HasAudio = true;

    int duration = MediaUtils::getDurationSec(m_MediaPath);
    if(duration == 0)
    {
        m_HasInvalidMedia = true;
        return;
    }

    if(m_Duration == 0)
        m_Duration = duration;

    SmilAudioItemView *item = new SmilAudioItemView(getBox(), media.getFileName());
    item->show();
    item->showIcon(true);
    appendItem(*item);
}

void SmilPage::buildVideo(const MsgMedia& media)
{
    m_MediaPath = media.getFilePath();

    int duration = MediaUtils::getDurationSec(m_MediaPath);
    if(duration == 0)
    {
        m_HasInvalidMedia = true;
        return;
    }
    if(m_Duration == 0)
        m_Duration = duration;

    int width = 0;
    int height = 0;
    MediaUtils::getFrameSize(m_MediaPath, width, height);

    if(width * height == 0)
    {
        MSG_LOG_ERROR("Wrong video dimension");
        m_HasInvalidMedia = true;
        return;
    }
    SmilVideoItemView *item = new SmilVideoItemView(getBox(), width, height);
    m_pVideoSink = item->getVideoSink();
    item->show();
    appendItem(*item);
}

void SmilPage::buildSaveAllItem(int attachmentCount)
{
    SmilSaveAllItemView *item = new SmilSaveAllItemView(getBox(), attachmentCount);
    item->setListener(this);
    appendItem(*item);
}

void SmilPage::buildAttachmentInfo(int attachmentCount)
{
    SmilAttachmentInfoItemView *item = new SmilAttachmentInfoItemView(getBox(), attachmentCount > 1);
    item->show();
    appendItem(*item);
}

void SmilPage::buildAttachment(const MsgAttachment& attachment)
{
    SmilAttachmentItemView *item = new SmilAttachmentItemView(getBox());
    item->setListener(this);
    item->setFilePath(attachment.getFilePath());
    item->setFileName(attachment.getFileName());
    item->show();
    appendItem(*item);
}

void SmilPage::onItemClicked(SmilAttachmentItemView &item)
{
    MSG_LOG("");
    m_FileViewer.launchWithCopy(item.getFilePath());
}

void SmilPage::onSaveButtonClicked(SmilAttachmentItemView &item)
{
    MSG_LOG("");
    if(FileUtils::saveFileToStorage(item.getFilePath()))
        notification_status_message_post(msg("IDS_MSGF_POP_SAVED_IN_MY_FILES").cStr());
    // TODO: else
}

void SmilPage::onItemClicked(SmilSaveAllItemView &item)
{
    MSG_LOG("");
    if(!m_Attachments.empty() && FileUtils::saveFilesToStorage(m_Attachments))
        notification_status_message_post(msg("IDS_MSGF_POP_SAVED_IN_MY_FILES").cStr());
    // TODO: else

}
