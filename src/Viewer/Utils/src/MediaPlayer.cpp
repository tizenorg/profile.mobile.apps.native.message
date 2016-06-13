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

#include "MediaPlayer.h"
#include "Logger.h"
#include "CallbackAssist.h"

#include <sound_manager.h>
#include <Ecore.h>

using namespace Msg;

MediaPlayer::MediaPlayer()
    : m_Player()
    , m_pListener(nullptr)
    , m_Focus(false)
{
    sound_manager_set_focus_state_watch_cb(SOUND_STREAM_FOCUS_FOR_PLAYBACK, on_sound_stream_focus_state_watch_cb, this);
    player_create(&m_Player);
    player_set_sound_type(m_Player, SOUND_TYPE_MEDIA);
    player_set_volume(m_Player, 1.0, 1.0);
    player_set_looping(m_Player, false);
    player_set_completed_cb(m_Player, on_completed_cb, this);
}

MediaPlayer::~MediaPlayer()
{
    sound_manager_unset_focus_state_watch_cb();
    m_pListener = nullptr;
    stop();
    player_unprepare(m_Player);
    player_destroy(m_Player);
}

void MediaPlayer::setDisplay(Evas_Object *obj)
{
    player_set_display_mode(m_Player, PLAYER_DISPLAY_MODE_LETTER_BOX);
    player_set_display(m_Player, PLAYER_DISPLAY_TYPE_EVAS, GET_DISPLAY(obj));
    player_set_display_visible(m_Player, obj != nullptr);
}

player_state_e MediaPlayer::getState() const
{
    player_state_e state = PLAYER_STATE_NONE;
    player_get_state(m_Player, &state);
    return state;
}

bool MediaPlayer::getFocus() const
{
    return m_Focus;
}

void MediaPlayer::start()
{
    if(getState() == PLAYER_STATE_IDLE)
        player_prepare(m_Player);

    if(getState() != PLAYER_STATE_PLAYING)
        player_start(m_Player);
}

void MediaPlayer::stop()
{
    player_state_e state = getState();
    if(state == PLAYER_STATE_PLAYING || state == PLAYER_STATE_PAUSED)
        player_stop(m_Player);
}

void MediaPlayer::pause()
{
    if(getState() == PLAYER_STATE_PLAYING)
        player_pause(m_Player);
}

bool MediaPlayer::isPlaying() const
{
    return getState() == PLAYER_STATE_PLAYING;
}

void MediaPlayer::setUri(const std::string &uri)
{
    stop();
    player_unprepare(m_Player);
    player_set_uri(m_Player, uri.c_str());
}

void MediaPlayer::setListener(IMediaPlayerListener *l)
{
    m_pListener = l;
}

int MediaPlayer::getDuration() const
{
    int msec = 0;
    player_get_duration(m_Player, &msec);
    return msec;
}

void MediaPlayer::setPosition(int msec)
{
    player_set_play_position(m_Player, msec, true, on_seek_cb, this);
}

int MediaPlayer::getDuration(const std::string &uri)
{
    int msec = 0;
    if(!uri.empty())
    {
        player_h player = {};
        player_create(&player);
        if(player_set_uri(player, uri.c_str()) == PLAYER_ERROR_NONE)
        {
            player_prepare(player);
            player_get_duration(player, &msec);
            player_unprepare(player);
        }
        player_destroy(player);
    }
    return msec;
}

void MediaPlayer::on_completed_cb(void *user_data)
{
    IMediaPlayerListener *l = static_cast<MediaPlayer*>(user_data)->m_pListener;
    if(l)
        l->onMediaPlayerCompleted();
}

void MediaPlayer::on_seek_cb(void *user_data)
{
    MSG_LOG("");
}

void MediaPlayer::on_sound_stream_focus_state_watch_cb(sound_stream_focus_mask_e focus_mask,
        sound_stream_focus_state_e focus_state, sound_stream_focus_change_reason_e reason,
        const char *extra_info, void *user_data)
{
    MSG_LOG("Interrupted focus state = ", focus_state);
    MSG_LOG("Interrupted focus change reason = ", reason);

    auto *self = static_cast<MediaPlayer*>(user_data);
    self->m_Focus = focus_state == SOUND_STREAM_FOCUS_STATE_RELEASED;

    ecore_main_loop_thread_safe_call_sync
    (
        [](void *data)->void*
        {
            auto *self = (MediaPlayer*)data;
            if(self->m_pListener)
                self->m_pListener->onMediaPlayerSoundFocusChanged();
            return nullptr;
        },
        self
    );

}
