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

#ifndef BaseController_h_
#define BaseController_h_

#include "BaseView.h"

namespace Msg
{
    class App;
    class MsgEngine;

    class BaseController
    {
        public:
            BaseController(App &app);
            BaseController(BaseController *parent);
            virtual ~BaseController();

            virtual BaseController &getParent();
            inline App &getApp();
            inline const App &getApp() const;
            MsgEngine &getMsgEngine();
            const MsgEngine &getMsgEngine() const;

        protected:
            virtual void onPause();
            virtual void onResume();

        private:
            BaseController(const BaseController&) = delete;
            BaseController& operator=(BaseController&) = delete;

        private:
            App &m_App;
            BaseController *m_pParent;
    };

    inline App &BaseController::getApp()
    {
        return m_App;
    }

    inline const App &BaseController::getApp() const
    {
        return m_App;
    }
}

#endif /* BaseController_h_ */
