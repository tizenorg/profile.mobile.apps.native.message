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
#include "ContactManager.h"
#include "MessageMms.h"
#include "MbeRecipients.h"
#include "ContactEditor.h"

namespace Msg
{
    class NaviFrameController;

    class Viewer
        : public FrameController
        , private IHwButtonListener
        , private IPlayerControlListener
        , private IContactManagerListener
        , private IViewerLayoutListener
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

            // IPlayerControlListener:
            virtual void onPlayClicked();
            virtual void onPauseClicked();
            virtual void onNextClicked();
            virtual void onPrevClicked();

            // IContactManagerListener:
            virtual void onContactChanged();

            // IViewerLayoutListener:
            virtual void onLayoutTocuh();

            // Popup:
            void onMakeVoiceItemPressed(PopupListItem &item);
            void onCreateContactItemPressed(PopupListItem &item);
            void onUpdateContactItemPressed(PopupListItem &item);

            // MbeRecipients:
            void onRecipItemClicked(Evas_Object *obj, void *eventInfo);

        private:
            void naviExpandButtonHandler();
            void naviCenterButtonHandler();
            void naviPrevButtonHandler();
            void updateNavibar();
            void updateRecipPanel();
            void create(MsgId id);
            void createLayout();
            void createPlayerControl();
            void createRecipPanel();
            void recipientClickHandler(const std::string &address);
            void showRecipPopup(const std::string &title);

        private:
            MessageMmsRef m_Msg;
            ViewerLayout *m_pLayout;
            PlayerControl *m_pPlayerControl;
            MbeRecipients *m_pRecipPanel;
            std::string m_SelectedAddress;
            ContactEditor m_ContactEditor;
    };
}

#endif /* ViewerFrame_h_ */
