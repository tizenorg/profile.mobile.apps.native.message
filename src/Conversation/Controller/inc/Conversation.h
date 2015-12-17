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

#include <Ecore.h>

namespace Msg
{
    class NaviFrameController;
    class Box;
    class Scroller;
    class ConversationInputPanel;

    class Conversation
        : public FrameController
        , private IHwButtonListener
        , private IMsgStorageListener
        , private IMessageInputPanelListener
        , private IListViewListener
        , private IBodyListener
        , private IRecipientsPanelListener
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

            // IMsgStorageListener:
            virtual void onMsgStorageChange(const MsgIdList &idList);

            // IHwButtonListener:
            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

            // IMessageInputPanelListener:
            virtual void onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id);

            // IRecipientsPanelListener:
            virtual void onKeyDown(RecipientsPanel &panel, Evas_Event_Key_Down &ev);
            virtual void onEntryFocusChanged(RecipientsPanel &panel);

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem, void *funcData);

            // IBodyListener:
            virtual void onChanged(Body &body);

        private:
            // Common:
            void create(Mode mode);
            void setMode(Mode mode);
            void setNewMessageMode();
            void setConversationMode();
            void createReicpPanel(Evas_Object *parent);
            void destroyRecipPanel();

            void sendMessage();
            void fillMessage(Message &msg);
            void fillMsgAddress(Message &msg);
            void saveDraftMsg();

            // Message input:
            void createMsgInput(Evas_Object *parent);
            void updateMsgInputPanel();

            // Bubble:
            void createBubbleList(Evas_Object *parent);
            void fillConversationList();

            // Predictive search:
            void createPredictSearch(Evas_Object *parent);
            void updateContactsList();
            void clearContactList();
            void updateContactsListRequest();
            bool onPredictSearchUpdateRequest();

        private:
            Mode m_Mode;
            ConversationLayout *m_pLayout;
            Scroller *m_pScroller;
            Box *m_pBubbleBox;
            ListView *m_pContactsList;
            MessageInputPanel *m_pMsgInputPanel;
            Body *m_pBody;
            RecipientsPanel *m_pRecipPanel;
            ThreadId m_ThreadId;
            Ecore_Idler *m_pPredictSearchIdler;
    };
}

#endif /* ConversationFrame_h_ */
