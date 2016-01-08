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

#ifndef Application_h_
#define Application_h_

#include "Window.h"
#include "MsgEngine.h"
#include "PopupManager.h"
#include <memory>

namespace Msg
{
    class MsgEngine;
    class ContactManager;

    class App
    {
        public:
            App();
            virtual ~App();

            virtual Window &getWindow() = 0;
            virtual const Window &getWindow() const = 0;

            PopupManager &getPopupManager();
            const PopupManager &getPopupManager() const;

            MsgEngine &getMsgEngine();
            const MsgEngine &getMsgEngine() const;

            ContactManager &getContactManager();
            const ContactManager &getContactManager() const;

            void exit();

        private:
            virtual void terminate() = 0;

        private:
            MsgEngine m_Engine;
            ContactManager *m_pContactManager;
            PopupManager *m_pPopupManager;
    };
}

#endif /* Application_h_ */
