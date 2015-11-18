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
#include "MsgStorage.h"
#include "ListView.h"
#include "Popup.h"
#include "ContextPopup.h"
#include "MsgThreadLayout.h"
#include "MsgThreadSearchPanel.h"

#include <string>
#include <memory>

namespace Msg
{
    class SelectAllListItem;
    class NaviFrameController;
    class ThreadListItem;
    class ThreadListView;
    class App;

    class MsgThread
        : public FrameController
        , private IHwButtonListener
        , private IListViewListener
        , private IMsgStorageListener
        , private IMsgThreadSearchPanelListener
    {
        public:
            MsgThread(NaviFrameController &parent);
            virtual ~MsgThread();

        private:
            // NaviFrameItem:
            virtual void onAttached(ViewItem &item);
            virtual void onButtonClicked(const NaviFrameItem &item, NaviButtonId buttonId);

            // IHwButtonListener:
            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

            // IMsgStorageListener:
            virtual void onMsgStorageChange();

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem, void *funcData);
            virtual void onListItemChecked(ListItem &listItem, void *funcData);

            // IPopupListener:
            virtual void onPopupButtonClicked(Popup &popup, int buttonId);

            // IContextPopupListener:
            virtual void onContextPopupItemPressed(ContextPopup &ctxPopup, ContextPopupItem &item);

            // IMsgThreadSearchPanelListener:
            virtual void onSearchButtonClicked(MsgThreadSearchPanel &obj);
            virtual void onEntryChanged(MsgThreadSearchPanel &obj);

        private:
            enum Mode
            {
                InitMode,
                NormalMode,
                DeleteMode,
                SearchMode
            };

        private:
            // MsgThread:
            void fillThreadList();
            void composeNewMessage();
            void navigateToSettings();
            void navigateToConversation(ThreadId threadId);
            void showMainCtxPopup();
            void setMode(Mode mode);
            void setDeleteMode(bool value);
            void setNormalMode();
            void deleteSelectedItems();

            void checkHandler(SelectAllListItem &item);
            void checkHandler(ThreadListItem &item);
            void selectHandler(ThreadListItem &item);

            // Search:
            Evas_Object *createSearchPanel(Evas_Object *parent);
            void setSearchMode(bool value);

        private:
            MsgThreadLayout *m_pLayout;
            ThreadListView *m_pThreadListView;
            MsgThreadSearchPanel *m_pSearchPanel;
            Mode m_Mode;
    };
}

#endif /* ThreadFrame_h_ */
