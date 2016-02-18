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

#ifndef NaviFrameController_h_
#define NaviFrameController_h_

#include "ViewController.h"
#include "NaviFrameView.h"
#include "HwButtonListener.h"
#include "AppControlCommand.h"
#include "AppControlCompose.h"
#include "AppControlDefault.h"

namespace Msg
{
    class FrameController;

    class NaviFrameController
        : public ViewController
        , public NaviFrameView
        , private IHwButtonListener
    {
        public:
            NaviFrameController(App &app);
            virtual ~NaviFrameController();

            /**
             * Pops last frame. Exit from app if frame count reaches zero.
             */
            void pop();

            /**
             * Executes app control command of default type.
             */
            void execCmd(const AppControlDefaultRef &cmd);

            /**
             * Executes app control command of compose type. This is Compose, Share, MultiShare, ShareText.
             */
            void execCmd(const AppControlComposeRef &cmd);

            /**
             * Destroys naviframe. Note, than this doesn't close app.
             */
            void destroy();

        private:
            void init();
            template<typename T>
            T *getTopFrame() const;
            bool execCmd(const AppControlCommand &cmd);

            bool isUnreadNotificationSingle(AppControlDefault::DefaultType type) const;

            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

        private:
            void registerHWButtonEvent();
    };
}

#endif /* NaviFrameController_h_ */
