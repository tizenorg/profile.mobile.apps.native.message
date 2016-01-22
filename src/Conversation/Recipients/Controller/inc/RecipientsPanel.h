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

#ifndef RecipientsPanel_h_
#define RecipientsPanel_h_

#include "RecipientViewItem.h"
#include "RecipientsPanelView.h"
#include "AppControlCompose.h"
#include "RecipientItem.h"
#include "Message.h"
#include "App.h"
#include "AppControlUtils.h"
#include "ContactPicker.h"

namespace Msg
{
    class IRecipientsPanelListener;

    class RecipientsPanel
        : public RecipientsPanelView,
          private IContactPickerListener
    {
        public:
            enum AppendItemStatus
            {
                SuccessStatus,
                DuplicatedStatus,
                InvalidRecipStatus
            };

        public:
            RecipientsPanel(Evas_Object *parent, App &app);
            virtual ~RecipientsPanel();

            void read(Message &msg);
            bool isMms() const;
            void setListener(IRecipientsPanelListener *l);
            void update(const ThreadId &threadId);
            void update(const MsgAddressListRef &addressList);
            AppendItemStatus appendItem(const std::string &address, MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType);
            AppendItemStatus appendItem(const std::string &address, const std::string &dispName,
                              MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType);
            void execCmd(const AppControlComposeRef &cmd);

        private:
            // RecipientsPanelView:
            virtual void onItemAdded(RecipientViewItem &item);
            virtual void onItemDeleted(RecipientViewItem &item);
            virtual void onItemSelected(RecipientViewItem &item);
            virtual void onItemClicked(RecipientViewItem &item);
            virtual void onKeyDown(Evas_Event_Key_Down *ev);
            virtual void onEntryFocusChanged();
            virtual void onContactButtonClicked();

            void onAppControlRes(app_control_h request, app_control_h reply, app_control_result_e result);
            void onPopupBtnClicked(Popup &popup, int buttonId);
            void onPopupDel(Evas_Object *popup, void *eventInfo);

            void addRecipientsFromEntry();
            void showDuplicatedRecipientPopup();
            void showTooManyRecipientsPopup();
            bool isRecipientExists(const std::string& address) const;

            // IContactPickerListener
            virtual void onContactsPicked(const std::list<int> &numberIdList);

        private:
            App &m_App;
            IRecipientsPanelListener *m_pListener;
            ContactPicker m_Picker;
    };

    class IRecipientsPanelListener
    {
        public:
            virtual ~IRecipientsPanelListener() {}

            virtual void onKeyDown(RecipientsPanel &panel, Evas_Event_Key_Down &ev) {};
            virtual void onEntryFocusChanged(RecipientsPanel &panel) {};
            virtual void onItemAdded(RecipientsPanel &panel, RecipientItem &item) {};
            virtual void onItemDeleted(RecipientsPanel &panel, RecipientItem &item) {};
    };
}

#endif /* RecipientsPanel_h_ */
