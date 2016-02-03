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
#include "RecipientsPanel.h"
#include "ListView.h"
#include "MsgEngine.h"
#include "ConvContactList.h"
#include "ConvList.h"
#include "AppControlCompose.h"
#include "AppControlDefault.h"
#include "AttachPanel.h"
#include "ContactEditor.h"

namespace Msg
{
    class NaviFrameController;
    class Scroller;
    class ConversationInputPanel;

    class Conversation
        : public FrameController
        , private IHwButtonListener
        , private IMsgStorageListener
        , private IMessageInputPanelListener
        , private IBodyListener
        , private IRecipientsPanelListener
        , private IConvContactListListener
        , private IConvListListener
        , private IAttachPanelListener
        , private IContactEditorListener
    {
        public:
            Conversation(NaviFrameController &parent);
            virtual ~Conversation();

            void navigateTo(MsgId msgId);
            void execCmd(const AppControlComposeRef &cmd);
            void execCmd(const AppControlDefaultRef &cmd);
            void setThreadId(ThreadId id);

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
            virtual void onItemAdded(RecipientsPanel &panel, RecipientItem &item);
            virtual void onItemDeleted(RecipientsPanel &panel, RecipientItem &item);
            virtual void onKeyDown(RecipientsPanel &panel, Evas_Event_Key_Down &ev);
            virtual void onEntryFocusChanged(RecipientsPanel &panel);
            virtual void onItemClicked(RecipientsPanel &panel, RecipientItem &item);

            // IBodyListener:
            virtual void onChanged(Body &body);

            // IConvContactListListener:
            virtual void onContactSelected(ContactListItem &item);

             // Popup callbacks:
            void onPopupDel(Evas_Object *popup, void *eventInfo);
            void onMsgSendErrorButtonClicked(Popup &popup, int buttonId);
            void onNoRecipCancelButtonClicked(Popup &popup, int buttonId);
            void onNoRecipDiscardButtonClicked(Popup &popup, int buttonId);

            // ContextPopup callbacks:
            void onDeleteItemPressed(ContextPopupItem &item);
            void onAddRecipientsItemPressed(ContextPopupItem &item);

            void onMakeVoiceItemPressed(PopupListItem &item);
            void onCreateContactItemPressed(PopupListItem &item);
            void onUpdateContactItemPressed(PopupListItem &item);

            // ConvList callbacks:
            virtual void onAllItemsDeleted(ConvList &list);
            virtual void onEditDraftMsg(MsgId id);
            virtual void onForwardMsg(MsgId id);

            // IAttachPanelListener:
            virtual void onFileSelected(AttachPanel &panel, const AttachPanel::FileList &files);

            // IContactEditorListener:
            virtual void onContactCreated(ContactEditor &obj);
            virtual void onContactChanged(ContactEditor &obj);


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

            void showMainCtxPopup();
            void showNoRecipPopup();
            void showRecipPopup(const std::string &title);
            void showSendResultPopup(MsgTransport::SendResult result);

            void sendMessage();
            void read(Message &msg);
            void readMsgAddress(Message &msg);
            void write(const Message &msg);
            void saveDraftMsg();
            void editDraftMsg(MsgId id);
            void forwardMsg(MsgId id);

            void onNaviOkButtonClicked();
            void onNaviCenterButtonClicked();
            void onNaviDownButtonClicked();

        private:
            Mode m_Mode;
            ConversationLayout *m_pLayout;
            MessageInputPanel *m_pMsgInputPanel;
            Body *m_pBody;
            RecipientsPanel *m_pRecipPanel;
            ConvContactList *m_pContactsList;
            ThreadId m_ThreadId;
            bool m_IsMms; // Compose message type
            ConvList *m_pConvList;
            AttachPanel m_AttachPanel;
            DefferedCmd m_DefferedCmd;
            std::string m_SelectedAddress;
            ContactEditor m_ContactEditor;
    };
}

#endif /* ConversationFrame_h_ */
