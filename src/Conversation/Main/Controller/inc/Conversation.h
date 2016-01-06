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
    {
        public:
            Conversation(NaviFrameController &parent);
            Conversation(NaviFrameController &parent, ThreadId threadId);
            virtual ~Conversation();

        private:
            enum Mode
            {
                InitMode,
                NewMessageMode,
                ConversationMode
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

            // IBodyListener:
            virtual void onChanged(Body &body);

            // IConvContactListListener:
            virtual void onContactSelected(ContactListItem &item);

        private:
            // Empty initializer constructor:
            Conversation(NaviFrameController &parent, bool dummy);
            void create(Mode mode);
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

            void notifyConvertMsgType();
            void convertMsgTypeHandler();
            void checkAndSetMsgType();

            void sendMessage();
            void fillMessage(Message &msg);
            void fillMsgAddress(Message &msg);
            void saveDraftMsg();

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
    };
}

#endif /* ConversationFrame_h_ */
