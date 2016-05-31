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
#include "Conversation.h"
#include "SmilPlayer.h"
#include "SubjectLayout.h"
#include "SystemSettingsManager.h"

namespace Msg
{
    class NaviFrameController;

    class Viewer
        : public FrameController
        , private IHwButtonListener
        , private IPlayerControlListener
        , private IContactManagerListener
        , private IViewerLayoutListener
        , private IConversationListener
        , private ISmilPlayerListener
        , private ISystemSettingsManager
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
            virtual void onLayoutTap();

            // ISmilPlayerListener:
            virtual void onSmilPlayerStateChanged();
            virtual void onSmilPlayerPageChanged();
            virtual void onSmilPlayerTick();

            // Popup:
            void onMakeVoiceItemPressed(PopupListItem &item);
            void onCreateContactItemPressed(PopupListItem &item);
            void onUpdateContactItemPressed(PopupListItem &item);

            // More Button popup
            void onDeleteItemPressed(PopupListItem &item);
            void onCancelButtonClicked(Popup &popup, int buttonId);
            void onDeleteButtonClicked(Popup &popup, int buttonId);
            void onCopyTextItemPressed(PopupListItem &item);
            void onForwardItemPressed(PopupListItem &item);
            void onShareItemPressed(PopupListItem &item);
            void onSaveAttachmentsItemPressed(PopupListItem &item);

            // MbeRecipients:
            void onRecipItemClicked(Evas_Object *obj, void *eventInfo);

            // Conversation listener
            virtual void onConversationSentMessage();

            // SystemSettingsManager:
            virtual void onLanguageChanged();

        private:
            void naviExpandButtonHandler();
            void naviCenterButtonHandler();
            void naviPrevButtonHandler();
            void updateNavibar();
            void updateRecipPanel();
            void create(MsgId id);
            void createLayout();
            void createSubjectLayout();
            void createPlayerControl();
            void createRecipPanel();
            std::string createMessageText() const;
            void createSmilPlayer();
            void recipientClickHandler(const std::string &address);
            void showRecipPopup(const std::string &title);
            void updatePlayPos();
            void updateSubject();
            void updateButtonState();

        private:
            MessageMmsRef m_Msg;
            ViewerLayout *m_pLayout;
            PlayerControl *m_pPlayerControl;
            MbeRecipients *m_pRecipPanel;
            std::string m_SelectedAddress;
            ContactEditor m_ContactEditor;
            SmilPlayer *m_pSmilPlayer;
            SubjectLayout *m_pSubjectLayout;
    };
}

#endif /* ViewerFrame_h_ */
