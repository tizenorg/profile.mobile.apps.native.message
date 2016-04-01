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
#include "SmilAttachmentItemView.h"
#include "Logger.h"
#include "FileUtils.h"
#include "MediaPlayer.h"
#include "MediaUtils.h"

using namespace Msg;

const int defaultPageDuration = 5; // sec;
std::string makeKbStr(long long bytes);

SmilPage::SmilPage(Evas_Object *parent, const MsgPage &page)
    : SmilPageLayout(parent)
    , m_Duration(0)
    , m_pVideoSink(nullptr)
    , m_pImageItem(nullptr)
{
    build(page);
}

SmilPage::SmilPage(Evas_Object *parent, const MsgAttachmentList &list)
    : SmilPageLayout(parent)
    , m_Duration(0)
    , m_pVideoSink(nullptr)
    , m_pImageItem(nullptr)
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

bool SmilPage::hasVideo() const
{
    return m_pVideoSink != nullptr;
}

Evas_Object *SmilPage::getVideoSink() const
{
    return m_pVideoSink;
}

std::string SmilPage::getMediaPath() const
{
    return m_MediaPath;
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

    // TODO: image/video, text order

    bool hasVideo = getMedia(page, MsgMedia::VideoType) != nullptr;

    if(hasVideo)
        buildVideo(*getMedia(page, MsgMedia::VideoType));

    if(!hasVideo)
    {
        if(const MsgMedia *image = getMedia(page, MsgMedia::ImageType))
            buildImage(*image);
    }

    if(const MsgMedia *text = getMedia(page, MsgMedia::TextType))
        buildText(*text);

    if(!hasVideo)
    {
        if(const MsgMedia *audio = getMedia(page, MsgMedia::AudioType))
            buildAudio(*audio);
    }

    if(m_Duration <= 0)
        m_Duration = defaultPageDuration;

    MSG_LOG("Page duration: ", m_Duration);
}

void SmilPage::build(const MsgAttachmentList &list)
{
    m_Duration = defaultPageDuration;

    if(list.isEmpty())
        return;

    buildAttachmentInfo(list.getLength());
    for(int i = 0; i < list.getLength(); ++i)
    {
        buildAttachment(list[i]);
    }
}

void SmilPage::buildImage(const MsgMedia &media)
{
    m_pImageItem = new SmilImageItemView(getBox(), media.getFilePath());
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

    if(m_Duration == 0)
        m_Duration = MediaUtils::getDurationSec(m_MediaPath);

    SmilAudioItemView *item = new SmilAudioItemView(getBox(), media.getFileName());
    item->show();
    item->showIcon(true);
    appendItem(*item);
}

void SmilPage::buildVideo(const MsgMedia& media)
{
    m_MediaPath = media.getFilePath();

    if(m_Duration == 0)
        m_Duration = MediaUtils::getDurationSec(m_MediaPath);

    int width = 0;
    int height = 0;
    MediaUtils::getFrameSize(m_MediaPath, width, height);

    if(width * height == 0)
    {
        // TODO: What to do in this case?
        MSG_LOG_ERROR("Wrong video dimension");
    }

    SmilVideoItemView *item = new SmilVideoItemView(getBox(), width, height);
    m_pVideoSink = item->getVideoSink();
    item->show();
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
    item->setFilePath(attachment.getFilePath());
    item->setFileName(attachment.getFileName());
    item->setFileSize(makeKbStr(attachment.getFileSize()));
    item->show();
    appendItem(*item);
}
