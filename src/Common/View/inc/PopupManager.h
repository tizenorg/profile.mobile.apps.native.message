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

    /**
     * @brief A global instance that manages context popups and regular popups lifecycle in context of one window.
     */
    class PopupManager
    {
        public:
            /**
             * @brief Creating PopupManager instance in context of specified window.
             * @param[in] window a window context.
             */
            PopupManager(Window &window);
            ~PopupManager();

            PopupManager(PopupManager&) = delete;
            PopupManager &operator=(PopupManager&) = delete;

            /**
             * @brief Gets window-context.
             * @return window-context.
             */
            Window &getWindow() const;

            /**
             * @brief Checks whether any context or regular popup is visible.
             * @return false if no popup is visible otherwise true.
             */
            bool isVisible() const;

            /**
             * @brief Destroys all popups (context and regular) created before.
             * If no popup was created nothing happens.
             */
            void reset();

            /**
             * @brief Destroys specified popup.
             * @param[in] popup a popup to be destroyed.
             */
            void reset(Popup &popup);

            /**
             * @brief Creates popup-list.
             * @return created popup-list.
             */
            PopupList &getPopupList();

            /**
             * @brief Creates popup.
             * @return popup created.
             */
            Popup &getPopup();

            /**
             * @brief Checks whether popup exists and it's visible.
             * @return true if popup exists and it's visible, false otherwise.
             */
            bool isPopupVisible() const;

            /**
             * @brief Destroys popup. If no popup exists nothing happens.
             */
            void resetPopup();

            /**
             * @brief Creates context popup.
             * @return context popup created.
             */
            ContextPopup &getCtxPopup();

            /**
             * @brief Checks whether context popup exists and it's visible.
             * @return true if context popup exists and it's visible, false otherwise.
             */
            bool isCtxPopupVisible() const;

            /**
             * @brief Destroys context popup. If no context popup exists nothing happens.
             */
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
