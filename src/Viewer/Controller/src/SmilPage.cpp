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
#include "Logger.h"
#include "FileUtils.h"
#include "MediaPlayer.h"

using namespace Msg;

const int defaultPageDuration = 5; // sec;

SmilPage::SmilPage(Evas_Object *parent, const MsgPage &page)
    : SmilPageLayout(parent)
    , m_Duration(0)
{
    build(page);
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

std::string SmilPage::getMediaPath() const
{
    return m_MediaPath;
}

const MsgMedia *SmilPage::getMedia(const MsgPage &page, MsgMedia::SmilType type) const
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

    // TODO: set image, text order
    if(const MsgMedia *image = getMedia(page, MsgMedia::SmilImage))
        buildImage(*image);
    if(const MsgMedia *text = getMedia(page, MsgMedia::SmilText))
        buildText(*text);
    if(const MsgMedia *audio = getMedia(page, MsgMedia::SmilAudio))
        buildAudio(*audio);

    if(m_Duration <= 0)
        m_Duration = defaultPageDuration;
}

void SmilPage::buildImage(const MsgMedia &media)
{
    SmilImageItemView *item = new SmilImageItemView(getEo(), media.getFilePath());
    item->show();
    appendItem(*item);
}

void SmilPage::buildText(const MsgMedia& media)
{
    std::string text = FileUtils::readTextFile(media.getFilePath());
    if(!text.empty())
    {
        SmilTextItemView *item = new SmilTextItemView(getEo(), text);
        item->show();
        appendItem(*item);
    }
}

void SmilPage::buildAudio(const MsgMedia& media)
{
    m_MediaPath = media.getFilePath();
    if(m_Duration == 0)
    {
        int sec = MediaPlayer::getDuration(m_MediaPath) / 1000.0 + 0.5;
        MSG_LOG("Duration: ", media.getFilePath(), " ", sec);

        if(m_Duration < sec)
            m_Duration = sec;
    }

    SmilAudioItemView *item = new SmilAudioItemView(getEo(), media.getFileName());
    item->show();
    item->showIcon(true);
    appendItem(*item);
}
