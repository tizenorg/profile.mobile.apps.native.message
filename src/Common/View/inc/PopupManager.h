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

#ifndef PopupManager_h_
#define PopupManager_h_

#include "Popup.h"
#include "PopupList.h"
#include "ContextPopup.h"

namespace Msg
{
    class Window;

    class PopupManager
    {
        public:
            PopupManager(Window &window);
            ~PopupManager();

            PopupManager(PopupManager&) = delete;
            PopupManager &operator=(PopupManager&) = delete;

            Window &getWindow() const;
            bool isVisible() const;
            void reset();
            void reset(Popup &popup);

            PopupList &getPopupList();
            Popup &getPopup();
            bool isPopupVisible() const;
            void resetPopup();

            ContextPopup &getCtxPopup();
            bool isCtxPopupVisible() const;
            void resetCtxPopup();

        private:
            void onHwBackButtonPopupClicked(Evas_Object *obj, void *eventInfo);
            void onHwBackButtonCtxPopupClicked(Evas_Object *obj, void *eventInfo);

        private:
            Popup *m_pPopup;
            ContextPopup *m_pCtxPopup;
            Window &m_Window;
    };
}

#endif /* PopupManager_h_ */
