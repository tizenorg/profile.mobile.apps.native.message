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

#include "SmilPlayer.h"
#include "Logger.h"
#include "CallbackAssist.h"
#include "LangUtils.h"

#include <notification.h>

using namespace Msg;

SmilPlayer::SmilPlayer(Evas_Object *parent, const MessageMms &mms)
    : SmilPlayerView(parent)
    , m_pListener(nullptr)
    , m_pTimer(nullptr)
    , m_CurrentPageIndex(0)
    , m_Duration(0)
    , m_State(StopState)
    , m_Finished(false)
    , m_PageTickCounter(0)
{
    create(mms);
}

SmilPlayer::~SmilPlayer()
{

}

void SmilPlayer::create(const MessageMms &mms)
{
    m_MediaPlayer.setListener(this);

    // Pages:
    const MsgPageList &pages = mms.getPageList();
    for(int i = 0; i < pages.getLength(); ++i)
    {
        SmilPage *smilPage = new SmilPage(getEo(), pages[i]);
        m_Duration += smilPage->getDuration();
        m_PageList.push_back(smilPage);
    }

    // Attachment:
    if(!mms.getAttachmentList().isEmpty())
    {
        SmilPage *smilPage = new SmilPage(getEo(), mms.getAttachmentList());
        m_Duration += smilPage->getDuration();
        m_PageList.push_back(smilPage);
    }

    if(!m_PageList.empty())
        displayPage(**m_PageList.begin());
}

void SmilPlayer::setListener(ISmilPlayerListener *l)
{
    m_pListener = l;
}

void SmilPlayer::startTimer(int sec)
{
    if(m_pTimer)
    {
        ecore_timer_del(m_pTimer);
        m_pTimer = nullptr;
    }
    m_PageTickCounter = 0;
    m_pTimer = ecore_timer_add(1.0, ECORE_TACK_CALLBACK(SmilPlayer, onTick), this);
}

void SmilPlayer::pauseTimer()
{
    if(m_pTimer)
        ecore_timer_freeze(m_pTimer);
}

void SmilPlayer::continueTimer()
{
    if(m_pTimer)
        ecore_timer_thaw(m_pTimer);
}

Eina_Bool SmilPlayer::onTick()
{
    MSG_LOG("");

    ++m_PageTickCounter;
    if(m_pListener)
        m_pListener->onSmilPlayerTick();

    if(m_PageTickCounter >= getCurrentPage()->getDuration())
    {
        m_pTimer = nullptr;

        if(!nextPage())
        {
            stop();
            m_Finished = true;
        }

        return false;
    }
    return true;
}

void SmilPlayer::setState(State state)
{
    if(m_State != state)
    {
        m_State = state;
        if(m_pListener)
            m_pListener->onSmilPlayerStateChanged();
    }
}

SmilPlayer::State SmilPlayer::getState() const
{
    return m_State;
}

bool SmilPlayer::isPlayFinished()
{
    return m_Finished;
}

void SmilPlayer::start()
{
    if(m_pTimer)
    {
        MSG_LOG("Continue");

        setState(PlayState);
        continueTimer();
        startMedia();
    }
    else
    {
        MSG_LOG("Restart");

        reset();
        setState(PlayState);
        playPage();
    }
}

void SmilPlayer::stop()
{
    pauseTimer();
    stopMedia();
    setState(StopState);
}

void SmilPlayer::reset()
{
    stop();
    m_Finished = false;
    m_CurrentPageIndex = 0;
    m_PageTickCounter = 0;
}

void SmilPlayer::playPage()
{
    m_Finished = false;
    SmilPage *page = getCurrentPage();
    SmilPlayerView::displayPage(*page);
    if(page->hasInvalidMedia())
        showNotSupportedFileNotif();

    prepareMedia();
    startMedia();
    startTimer(page->getDuration());
    if(m_pListener)
        m_pListener->onSmilPlayerPageChanged();
}

void SmilPlayer::prepareMedia()
{
    m_MediaPlayer.stop();
    SmilPage *page = getCurrentPage();
    if(page->hasMedia())
    {
        m_MediaPlayer.setUri(page->getMediaPath());
        Evas_Object *videoSink = page->getVideoSink();
        m_MediaPlayer.setDisplay(videoSink);
    }
}

void SmilPlayer::stopMedia()
{
    m_MediaPlayer.pause();
    getCurrentPage()->playAnimation(false);
}

void SmilPlayer::startMedia()
{
    if(getCurrentPage()->hasMedia())
        m_MediaPlayer.start();
    getCurrentPage()->playAnimation(true);
}

double SmilPlayer::getPosition() const
{
    unsigned pos = 0;
    for(unsigned i = 0; i < m_CurrentPageIndex; ++i)
    {
        pos += m_PageList[i]->getDuration();
    }

    pos += m_PageTickCounter;

    return pos / (double)m_Duration;
}

const SmilPlayer::PageList &SmilPlayer::getPages() const
{
    return m_PageList;
}

SmilPage *SmilPlayer::getCurrentPage() const
{
    return m_PageList[m_CurrentPageIndex];
}

int SmilPlayer::getDuration() const
{
    return m_Duration;
}

bool SmilPlayer::nextPage()
{
    if(probeNextPage())
    {
        ++m_CurrentPageIndex;
        playPage();
        return true;
    }
    return false;
}

bool SmilPlayer::prevPage()
{
    if(probePrevPage())
    {
        --m_CurrentPageIndex;
        playPage();
        return true;
    }
    return false;
}

bool SmilPlayer::probeNextPage() const
{
    return m_CurrentPageIndex < m_PageList.size() - 1;
}

bool SmilPlayer::probePrevPage() const
{
    return m_CurrentPageIndex > 0;
}

unsigned SmilPlayer::getCurrentPageIndex() const
{
    return m_CurrentPageIndex;
}

void SmilPlayer::showUnableToPlayVideoNotif()
{
    notification_status_message_post(msg("IDS_MSG_TPOP_CANT_PLAY_VIDEOS_DURING_CALLS").cStr());
}

void SmilPlayer::showUnableToPlayAudioNotif()
{
    notification_status_message_post(msg("IDS_MSG_TPOP_CANT_PLAY_AUDIO_FILES_DURING_CALLS").cStr());
}

void SmilPlayer::showNotSupportedFileNotif()
{
    notification_status_message_post(msg("IDS_MSG_TPOP_CANT_PREVIEW_FILE_FILE_FORMAT_NOT_SUPPORTED").cStr());
}

void SmilPlayer::onBeforeDelete(View &view)
{
    MSG_LOG("");

    m_pListener = nullptr;
    stop();

    for(SmilPage *page : m_PageList)
    {
        page->destroy();
    }

    if(m_pTimer)
    {
        ecore_timer_del(m_pTimer);
        m_pTimer = nullptr;
    }
}

void SmilPlayer::onMediaPlayerSoundFocusChanged()
{
    if(m_MediaPlayer.isPlaying() && m_MediaPlayer.isFocusChangedCallReason())
    {
        SmilPage *page = getCurrentPage();
        if(page)
        {
            m_MediaPlayer.pause();
            if(page->hasVideo())
                showUnableToPlayVideoNotif();
            else if(page->hasAudio())
                showUnableToPlayAudioNotif();
            if(page->hasMedia())
                stop();
        }
    }
}
