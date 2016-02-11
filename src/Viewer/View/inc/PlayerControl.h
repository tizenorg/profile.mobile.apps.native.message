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

#ifndef PlayerControl_h_
#define PlayerControl_h_

#include "View.h"

namespace Msg
{
    class IPlayerControlListener;

    class PlayerControl
        : public View
    {
        public:
            enum PlayPauseState
            {
                PlayState,
                PauseState
            };

        public:
            PlayerControl(Evas_Object *parent);
            virtual ~PlayerControl();

            void setListener(IPlayerControlListener *l);
            void setProgress(double value); // 0.0..1.0
            void prevButtonEnable(bool enable);
            void nextButtonEnable(bool enable);
            void setPlayState(PlayPauseState state);
            void setStartTime(const std::string &time);
            void setEndTime(const std::string &time);

        private:
            void onPlay(Evas_Object *obj, const char *emission, const char *source);
            void onPause(Evas_Object *obj, const char *emission, const char *source);
            void onNext(Evas_Object *obj, const char *emission, const char *source);
            void onPrev(Evas_Object *obj, const char *emission, const char *source);

            Evas_Object *createPlayer(Evas_Object *parent);
            Evas_Object *createProgress(Evas_Object *parent);

        private:
            IPlayerControlListener *m_pListener;
            Evas_Object *m_pProgress;
    };

    class IPlayerControlListener
    {
        public:
            virtual ~IPlayerControlListener() {}

            virtual void onPlayClicked() {};
            virtual void onPauseClicked() {};
            virtual void onNextClicked() {};
            virtual void onPrevClicked() {};
    };
}

#endif // PlayerControl_h_
