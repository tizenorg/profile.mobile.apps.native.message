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

#ifndef ThreadFrame_h_
#define ThreadFrame_h_

#include "FrameController.h"
#include "HwButtonListener.h"
#include "Popup.h"
#include "ContextPopup.h"
#include "MsgThreadLayout.h"
#include "MsgThreadSearchPanel.h"
#include "FloatingButton.h"
#include "NoContentLayout.h"
#include "MsgTypes.h"
#include "ThreadList.h"
#include "ThreadSearchList.h"
#include "SystemSettingsManager.h"

#include <string>
#include <memory>

namespace Msg
{
    class SelectAllListItem;
    class NaviFrameController;
    class ThreadListItem;
    class App;

    class MsgThread
        : public FrameController
        , private IHwButtonListener
        , private IMsgThreadSearchPanelListener
        , private IFloatingButtonListener
        , private IThreadListListener
        , private IThreadSearchListListener
        , private ISystemSettingsManager
    {
        public:
            MsgThread(NaviFrameController &parent);
            virtual ~MsgThread();

        private:
            // NaviFrameItem:
            virtual void onAttached(ViewItem &item);
            virtual void onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId);

            // IHwButtonListener:
            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

            // Popup callbacks:
            void onSettingsItemPressed(PopupListItem &item);
            void onDeleteItemPressed(PopupListItem &item);
            void onSearchItemPressed(PopupListItem &item);

            // IMsgThreadSearchPanelListener:
            virtual void onSearchButtonClicked(MsgThreadSearchPanel &obj);
            virtual void onEntryChanged(MsgThreadSearchPanel &obj);

            // IFloatingButtonListener
            virtual void onFloatingButtonPressed();

            // IThreadListListener:
            virtual void onListItemSelected(ThreadId id);
            virtual void onThreadListChanged();
            virtual void onThreadListItemChecked();

            // ThreadSearchList:
            virtual void onSearchListChanged();
            virtual void onSearchListItemSelected(ThreadId id);
            virtual void onSearchListItemSelected(MsgId id, const std::string &searchWord);

            // ISystemSettingsManager:
            virtual void onLanguageChanged();

        private:
            enum Mode
            {
                InitMode,
                DeleteMode,
                NormalMode,
                SearchMode
            };

        private:
            // MsgThread:
            void composeNewMessage();
            void navigateToSettings();
            void navigateToConversation(ThreadId threadId, MsgId msgId = MsgId(), const std::string &searchWord = std::string());
            void showMainPopup();
            void setMode(Mode mode);
            void setDeleteMode(bool value);
            void setNormalMode();
            void update();
            void updateSelectItemsTitle();
            void search(const std::string &searchWord);

            // Search:
            Evas_Object *createSearchPanel(Evas_Object *parent);
            void setSearchMode(bool value);

        private:
            MsgThreadLayout *m_pLayout;
            NoContentLayout *m_pNoContent;
            ThreadList *m_pThreadList;
            ThreadSearchList *m_pSearchList;
            MsgThreadSearchPanel *m_pSearchPanel;
            Mode m_Mode;
            FloatingButton *m_pFloatingBtn;
    };
}

#endif /* ThreadFrame_h_ */
