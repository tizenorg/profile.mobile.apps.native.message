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

#ifndef SmilPlayer_h_
#define SmilPlayer_h_

#include "SmilPlayerView.h"
#include "MessageMms.h"
#include "SmilPage.h"
#include "MediaPlayer.h"

#include <Ecore.h>
#include <vector>

namespace Msg
{
    class ISmilPlayerListener;

    class SmilPlayer
        : public SmilPlayerView
    {
        public:
            typedef std::vector<SmilPage*> PageList;
            enum State
            {
                StopState, // Stop = Pause
                PlayState,
            };

        public:
            SmilPlayer(Evas_Object *parent, const MessageMms &mms);
            virtual ~SmilPlayer();

            void setListener(ISmilPlayerListener *l);
            State getState() const;
            bool isPlayFinished();
            void start();
            void stop();
            void reset();
            double getPosition() const;
            const PageList &getPages() const;
            SmilPage *getCurrentPage() const;
            int getDuration() const;
            bool nextPage();
            bool prevPage();
            bool probeNextPage() const;
            bool probePrevPage() const;
            unsigned getCurrentPageIndex() const;

        private:
            void playPage();
            void startMedia();
            void stopMedia();
            void prepareMedia();
            void create(const MessageMms &mms);
            void startTimer(int sec);
            void pauseTimer();
            void continueTimer();
            Eina_Bool onTick();
            void setState(State state);
            virtual void onBeforeDelete(View &view);

        private:
            ISmilPlayerListener *m_pListener;
            Ecore_Timer *m_pTimer;
            MediaPlayer m_MediaPlayer;
            PageList m_PageList;
            unsigned m_CurrentPageIndex;
            int m_Duration;
            State m_State;
            bool m_Finished;
            int m_PageTickCounter;
    };

    class ISmilPlayerListener
    {
        public:
            virtual ~ISmilPlayerListener() {}
            virtual void onSmilPlayerPageChanged() {};
            virtual void onSmilPlayerStateChanged() {};
            virtual void onSmilPlayerTick() {};
    };
}

#endif /* SmilPlayer_h_ */
