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

#ifndef ViewerFrame_h_
#define ViewerFrame_h_

#include "FrameController.h"
#include "HwButtonListener.h"
#include "ViewerLayout.h"
#include "PlayerControl.h"
#include "MsgTypes.h"

namespace Msg
{
    class NaviFrameController;

    class Viewer
        : public FrameController
        , private IHwButtonListener
        , private IPlayerControlListener
    {
        public:
            Viewer(NaviFrameController &parent, MsgId id);
            virtual ~Viewer();

        private:

            // NaviFrameItem:
            virtual void onAttached(ViewItem &item);
            virtual void onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId);

            // IHwButtonListener:
            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

            /// IPlayerControlListener:
            virtual void onPlayClicked();
            virtual void onPauseClicked();
            virtual void onNextClicked();
            virtual void onPrevClicked();

        private:
            void updateNavibar();
            void create();
            void createLayout();
            void createPlayerControl();

        private:
            MsgId m_MsgId;
            ViewerLayout *m_pLayout;
            PlayerControl *m_pPlayerControl;
    };
}

#endif /* ViewerFrame_h_ */
