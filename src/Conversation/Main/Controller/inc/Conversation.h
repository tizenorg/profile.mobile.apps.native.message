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

#ifndef ConversationFrame_h_
#define ConversationFrame_h_

#include "FrameController.h"
#include "ConversationLayout.h"
#include "HwButtonListener.h"
#include "MessageInputPanel.h"
#include "Body.h"
#include "MsgThreadItem.h"
#include "ConvRecipientsPanel.h"
#include "ListView.h"
#include "MsgEngine.h"
#include "ConvContactList.h"
#include "ConvList.h"
#include "AppControlCompose.h"
#include "AppControlDefault.h"
#include "AttachPanel.h"
#include "ContactEditor.h"
#include "ContactManager.h"
#include "SystemSettingsManager.h"
#include "WorkingDir.h"

namespace Msg
{
    class NaviFrameController;
    class Scroller;
    class ConversationInputPanel;
    class IConversationListener;

    class Conversation
        : public FrameController
        , private IHwButtonListener
        , private IMessageInputPanelListener
        , private IBodyListener
        , private IConvRecipientsPanelListener
        , private IConvContactListListener
        , private IConvListListener
        , private IAttachPanelListener
        , private IContactManagerListener
        , private ISystemSettingsManager
    {
        public:
            Conversation(NaviFrameController &parent);
            virtual ~Conversation();

            void navigateTo(MsgId msgId);
            void navigateToLastMsg();
            void execCmd(const AppControlComposeRef &cmd);
            void execCmd(const AppControlDefaultRef &cmd);
            void setThreadId(ThreadId id, const std::string &searchWord = std::string());
            void setListener(IConversationListener *listener);
            void forwardMsg(MsgId id);

        private:
            enum Mode
            {
                InitMode,
                NewMessageMode,
                ConversationMode
            };

            struct DefferedCmd
            {
                AppControlComposeRef composeCmd;
                AppControlDefaultRef defaultCmd;
            };

        private:
            // NaviFrameItem:
            virtual void onAttached(ViewItem &item);
            virtual void onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId);

            // IHwButtonListener:
            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

            // IMessageInputPanelListener:
            virtual void onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id);

            // IRecipientsPanelListener:
            virtual void onMbeChanged(ConvRecipientsPanel &panel);
            virtual void onKeyDown(ConvRecipientsPanel &panel, Evas_Event_Key_Down &ev);
            virtual void onEntryFocusChanged(ConvRecipientsPanel &panel);
            virtual void onItemClicked(ConvRecipientsPanel &panel, MbeRecipientItem &item);

            // IBodyListener:
            virtual void onChanged(Body &body);

            // IConvContactListListener:
            virtual void onContactSelected(ContactListItem &item);
            virtual void onContactListChanged();

             // Popup callbacks:
            void onPopupDel(Evas_Object *popup, void *eventInfo);
            void onMsgSendErrorButtonClicked(Popup &popup, int buttonId);
            void onMsgSettingsButtonClicked(Popup &popup, int buttonId);
            void onNoRecipCancelButtonClicked(Popup &popup, int buttonId);
            void onNoRecipDiscardButtonClicked(Popup &popup, int buttonId);
            void onDeleteItemPressed(PopupListItem &item);
            void onAddRecipientsItemPressed(PopupListItem &item);
            void onMakeVoiceItemPressed(PopupListItem &item);
            void onCreateContactItemPressed(PopupListItem &item);
            void onUpdateContactItemPressed(PopupListItem &item);
            void onRecipRemoveItemPressed(PopupListItem &item);
            void onEditItemPressed(PopupListItem &item);
            void onViewContactDetailsItemPressed(PopupListItem &item);

            // ConvList callbacks:
            virtual void onAllItemsDeleted(ConvList &list);
            virtual void onEditDraftMsg(MsgId id);
            virtual void onForwardMsg(MsgId id);
            virtual void onSlideShow(MsgId id);
            virtual void onConvListItemChecked();

            // IAttachPanelListener:
            virtual void onFileSelected(AttachPanel &panel, const AttachPanel::FileList &files);

            // IContactManagerListener:
            virtual void  onContactChanged();

            // ISystemSettingsManager:
            virtual void onLanguageChanged();

        private:
            void create();
            void setMode(Mode mode);
            void setNewMessageMode();
            void setConversationMode();
            void createBody(Evas_Object *parent);
            void createRecipPanel(Evas_Object *parent);
            void destroyRecipPanel();
            void createContactList(Evas_Object *parent);
            void destroyContactList();
            void createMsgInputPanel(Evas_Object *parent);
            void updateMsgInputPanel();
            void updateNavibar();
            void updateSelectMsgTitle();
            void createMainLayout(Evas_Object *parent);
            void createConvList(Evas_Object *parent);
            void destroyConvList();
            void markAsRead();
            void recipientClickHandler(const std::string &address);
            void contactChangedHandler();
            MsgAddressListRef getAddressList();
            void resetDefferedCmd();
            bool isDefferedCmd() const;
            bool isRecipExists() const;
            bool isBodyEmpty() const;
            void notifyConvertMsgType();
            void convertMsgTypeHandler();
            void checkAndSetMsgType();
            void navigateToSlideShow(MsgId id);

            void showMainPopup();
            void showNoRecipPopup();
            PopupList &createPopupList(const std::string &title);
            void showSendResultPopup(MsgTransport::SendResult result);
            void showMobileDataPopup();
            void showUnsavedRecipientPopup(const std::string &address);
            void showSavedRecipientPopup(const std::string &title, int contactId, ContactAddress::OwnerType ownerType);
            void sendMessage();
            bool read(Message &msg);
            bool readMsgAddress(Message &msg);
            void write(const Message &msg);
            void saveDraftMsg();
            void editDraftMsg(MsgId id);

            void onNaviOkButtonClicked();
            void onNaviCenterButtonClicked();
            void onNaviExpandButtonClicked();

        private:
            Mode m_Mode;
            WorkingDirRef m_WorkingDir;
            ConversationLayout *m_pLayout;
            MessageInputPanel *m_pMsgInputPanel;
            Body *m_pBody;
            ConvRecipientsPanel *m_pRecipPanel;
            ConvContactList *m_pContactsList;
            ThreadId m_ThreadId;
            bool m_IsMms; // Compose message type
            ConvList *m_pConvList;
            AttachPanel m_AttachPanel;
            DefferedCmd m_DefferedCmd;
            ContactEditor m_ContactEditor;
            IConversationListener *m_pListener;
    };

    class IConversationListener
    {
        public:
            virtual ~IConversationListener() {};
            virtual void onConversationSentMessage() {};
    };
}

#endif /* ConversationFrame_h_ */
