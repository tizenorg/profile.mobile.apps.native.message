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

#include "PlayerControl.h"
#include "Resource.h"
#include "CallbackAssist.h"

using namespace Msg;

namespace
{
    const char *groupName = "player";
    const char *prevCb = "prev_clicked";
    const char *playCb= "play_clicked";
    const char *pauseCb = "pause_clicked";
    const char *nextCb = "next_clicked";
    const char *progressSwl = "progress_area";
    const char *playSig = "play";
    const char *pauseSig = "pause";
    const char *prevEnableSig = "prev_enable";
    const char *prevDisableSig = "prev_disable";
    const char *nextEnableSig = "next_enable";
    const char *nextDisableSig = "next_disable";
    const char *startTime = "progress/start/time";
    const char *endTime = "progress/end/time";
}

PlayerControl::PlayerControl(Evas_Object *parent)
    : m_pListener(nullptr)
    , m_pProgress(nullptr)
{
    setEo(createPlayer(parent));
    Evas_Object *progress = createProgress(getEo());
    setContent(progress, progressSwl);
}

PlayerControl::~PlayerControl()
{

}

void PlayerControl::setListener(IPlayerControlListener *l)
{
    m_pListener = l;
}

void PlayerControl::setProgress(double percentage)
{
    elm_progressbar_value_set(m_pProgress, percentage);
}

Evas_Object *PlayerControl::createPlayer(Evas_Object *parent)
{
    Evas_Object *layout = addLayout(parent, VIEWER_PALYER_EDJ_PATH, groupName);
    elm_object_signal_callback_add(layout, playCb, "", EDJE_SIGNAL_CALLBACK(PlayerControl, onPlay), this);
    elm_object_signal_callback_add(layout, pauseCb, "",  EDJE_SIGNAL_CALLBACK(PlayerControl, onPause), this);
    elm_object_signal_callback_add(layout, nextCb, "",  EDJE_SIGNAL_CALLBACK(PlayerControl, onNext), this);
    elm_object_signal_callback_add(layout, prevCb, "", EDJE_SIGNAL_CALLBACK(PlayerControl, onPrev), this);
    return layout;
}

Evas_Object *PlayerControl::createProgress(Evas_Object *parent)
{
    Evas_Object *progressbar = elm_progressbar_add(parent);
    elm_object_style_set(progressbar, "list_progress");
    elm_progressbar_horizontal_set(progressbar, true);
    elm_progressbar_unit_format_set(progressbar, nullptr);
    evas_object_show(progressbar);
    return progressbar;
}

void PlayerControl::prevButtonEnable(bool enable)
{
    const char *sig = enable ? prevEnableSig : prevDisableSig;
    emitSignal(sig, "*");
}

void PlayerControl::nextButtonEnable(bool enable)
{
    const char *sig = enable ? nextEnableSig : nextDisableSig;
    emitSignal(sig, "*");
}

void PlayerControl::setPlayState(PlayPauseState state)
{
    const char *sig = state == PlayState ? playSig : pauseSig;
    emitSignal(sig, "*");
}

void PlayerControl::setStartTime(const std::string &time)
{
    setText(time, startTime);
}

void PlayerControl::setEndTime(const std::string &time)
{
    setText(time, endTime);
}

void PlayerControl::onPlay(Evas_Object *obj, const char *emission, const char *source)
{
    if(m_pListener)
        m_pListener->onPlayClicked();
}

void PlayerControl::onPause(Evas_Object *obj, const char *emission, const char *source)
{
    if(m_pListener)
        m_pListener->onPauseClicked();
}

void PlayerControl::onNext(Evas_Object *obj, const char *emission, const char *source)
{
    if(m_pListener)
        m_pListener->onNextClicked();
}

void PlayerControl::onPrev(Evas_Object *obj, const char *emission, const char *source)
{
    if(m_pListener)
        m_pListener->onPrevClicked();
}

