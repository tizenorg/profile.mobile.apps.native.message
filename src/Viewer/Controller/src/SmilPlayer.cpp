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
    MSG_LOG("");
    m_pListener = nullptr;
    stop();
}

void SmilPlayer::create(const MessageMms &mms)
{
    const MsgPageList &pages = mms.getPageList();
    for(int i = 0; i < pages.getLength(); ++i)
    {
        SmilPage *smilPage = new SmilPage(getEo(), pages[i]);
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
    if(!m_pTimer)
    {
        m_PageTickCounter = 0;
        m_pTimer = ecore_timer_add(1.0, ECORE_TACK_CALLBACK(SmilPlayer, onTick), this);
    }
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
        nextPage();
        return false;
    }
    return true;
}

void SmilPlayer::setState(State state)
{
    m_State = state;
    if(m_pListener)
        m_pListener->onSmilPlayerStateChanged();
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
        setState(PlayState);
        continueTimer();
    }
    else
    {
        if(m_Finished)
        {
            m_Finished = false;
            m_CurrentPageIndex = 0;
        }
        setState(PlayState);
        playPage();
    }
}

void SmilPlayer::stop()
{
    setState(StopState);
    pauseTimer();
}

void SmilPlayer::reset()
{
    m_Finished = false;
    m_PageTickCounter = 0;
    stop();
}

void SmilPlayer::playPage()
{
    stopMedia();
    m_Finished = false;
    SmilPage *page = getCurrentPage();
    displayPage(*page);
    playMedia("");
    startTimer(page->getDuration());
    if(m_pListener)
        m_pListener->onSmilPlayerPageChanged();
}

void SmilPlayer::stopPage()
{
    pauseTimer();
    stopMedia();
}

void SmilPlayer::playMedia(const std::string &res)
{
    // TODO: impl
}

void SmilPlayer::stopMedia()
{
    // TODO: impl
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

void SmilPlayer::nextPage()
{
    if(m_CurrentPageIndex < m_PageList.size() - 1)
    {
        ++m_CurrentPageIndex;
        playPage();
    }
    else
    {
        stop();
        m_Finished = true;
    }
}

void SmilPlayer::prevPage()
{
    if(m_CurrentPageIndex > 0)
    {
        --m_CurrentPageIndex;
        playPage();
    }
    else
    {
        stop();
    }
}

bool SmilPlayer::probeNextPage() const
{
    return m_CurrentPageIndex < m_PageList.size() - 1;
}

bool SmilPlayer::probePrevPage() const
{
    return m_CurrentPageIndex > 0;
}

void SmilPlayer::onBeforeDelete(View &view)
{
    MSG_LOG("");

    for(SmilPage *page : m_PageList)
    {
        page->destroy();
    }
}
