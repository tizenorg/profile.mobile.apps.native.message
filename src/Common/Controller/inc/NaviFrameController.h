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

            void push(FrameController &frame);
            void pop();
            void executeCommand(AppControlCommandRef &cmd);
            void destroy();

        private:
            void init();

            void execCmd(AppControlCommandDefaultRef cmd);
            void execCmd(AppControlCommandRef cmd);

            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

        private:
            void registerHWButtonEvent();
    };
}

#endif /* NaviFrameController_h_ */
