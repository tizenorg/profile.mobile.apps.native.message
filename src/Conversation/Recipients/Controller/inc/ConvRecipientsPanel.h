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

#include "ConvRecipientsPanelView.h"
#include "AppControlCompose.h"
#include "Message.h"
#include "App.h"
#include "AppControlUtils.h"
#include "ContactPicker.h"
#include "ContactManager.h"

namespace Msg
{
    class IConvRecipientsPanelListener;

    class ConvRecipientsPanel
        : public ConvRecipientsPanelView
        , private IContactPickerListener
        , private IMbeRecipientsListener
        , private IContactManagerListener
    {
        public:
            ConvRecipientsPanel(Evas_Object *parent, App &app);
            virtual ~ConvRecipientsPanel();

            void read(Message &msg);
            void write(const Message &msg);
            bool isMms() const;
            void setListener(IConvRecipientsPanelListener *l);
            void update(const ThreadId &threadId);
            void update(const MsgAddressList &addressList);
            void execCmd(const AppControlComposeRef &cmd);
            MbeRecipients::AppendItemStatus appendItem(const std::string &address, MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType);
            void removeSelectedItem();
            void editSelectedItem();
            bool addRecipientsFromEntry(bool showPopup = true);

        private:
            // RecipientsPanelView:
            virtual void onKeyDown(Evas_Event_Key_Down *ev);
            virtual void onEntryFocusChanged();
            virtual void onContactButtonClicked();
            virtual void onPlusButtonClicked();
            virtual void onEntryChanged();

            // IMbeRecipientsListener
            virtual void onMbeItemClicked(MbeRecipientItem &item);
            virtual void onMbeChanged();

            void onAppControlRes(app_control_h request, app_control_h reply, app_control_result_e result);
            void onPopupBtnClicked(Popup &popup, int buttonId);
            void onPopupDel(Evas_Object *popup, void *eventInfo);

            void appendStatusHandler(MbeRecipients::AppendItemStatus status);
            int getMaxRecipientCount() const;
            void showInvalidRecipientPopup();
            void showInvalidSomeRecipientsPopup();
            void showInvalidAllRecipientsPopup();
            void showTooManyRecipientsNotif();
            void showAddRecipNotif();
            void showDuplicatedRecipientNotif();
            MbeRecipients::AppendItemStatus appendItem(const std::string &address, const std::string &dispName,
                              MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType);

            // IContactPickerListener
            virtual void onContactsPicked(const std::list<int> &numberIdList);

            // IContactManagerListener:
            virtual void onContactChanged();

        private:
            App &m_App;
            IConvRecipientsPanelListener *m_pListener;
            ContactPicker m_Picker;
            MbeRecipients *m_pMbe;
    };

    class IConvRecipientsPanelListener
    {
        public:
            virtual ~IConvRecipientsPanelListener() {}

            virtual void onKeyDown(ConvRecipientsPanel &panel, Evas_Event_Key_Down &ev) {};
            virtual void onEntryFocusChanged(ConvRecipientsPanel &panel) {};
            virtual void onMbeChanged(ConvRecipientsPanel &panel) {};
            virtual void onItemClicked(ConvRecipientsPanel &panel, MbeRecipientItem &item) {};
    };
}

#endif /* RecipientsPanel_h_ */
