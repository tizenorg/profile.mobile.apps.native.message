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

#ifndef MainApp_h_
#define MainApp_h_

#include "App.h"

#include <app.h>
#include <app_control.h>
#include <memory>

namespace Msg
{
    class StandardWindow;
    class NaviFrameController;

    class MainApp:
        public App
    {
        public:
            MainApp();
            virtual ~MainApp();

            int start(int argc, char *argv[]);

            virtual Window &getWindow();
            virtual const Window &getWindow() const;

        private:
            virtual void terminate();

            // App cbs:
            bool onAppCreate();
            void onAppTerminate();
            void onAppPause();
            void onAppResume();
            void onAppControl(app_control_h app_control);
            void onLanguageChanged(app_event_info_h appEvent);
            void onLowBattery(app_event_info_h appEvent);
            void onRegionChanged(app_event_info_h appEvent);

        private:
            StandardWindow *m_pWindow;
            NaviFrameController *m_pRootController;
    };
}

#endif /* MainApp_h_ */
