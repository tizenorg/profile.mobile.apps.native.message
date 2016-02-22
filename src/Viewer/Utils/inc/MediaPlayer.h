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

#ifndef MediaPlayer_h_
#define MediaPlayer_h_

#include <player.h>
#include <string>

namespace Msg
{
    class IMediaPlayerListener;

    class MediaPlayer
    {
        public:
            MediaPlayer();
            ~MediaPlayer();
            MediaPlayer(const MediaPlayer&) = delete;
            MediaPlayer& operator=(const MediaPlayer&) = delete;

            void start();
            void stop();
            void pause();
            bool isPlaying() const;
            void setUri(const std::string &uri);
            void setListener(IMediaPlayerListener *l);
            int getDuration() const; // msec
            void setPosition(int msec);
            static int getDuration(const std::string &uri);

        private:
            static void on_completed_cb(void *user_data);
            static void on_seek_cb(void *user_data);
            player_state_e getState() const;

        private:
            player_h m_Player;
            IMediaPlayerListener *m_pListener;
    };

    class IMediaPlayerListener
    {
        public:
            virtual ~IMediaPlayerListener() {}
            virtual void onMediaPlayerCompleted() {};
    };
}

#endif // MediaPlayer_h_
