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
#include "ConvRecipientItem.h"
#include "Message.h"
#include "App.h"
#include "AppControlUtils.h"
#include "ContactPicker.h"

namespace Msg
{
    class IConvRecipientsPanelListener;

    class ConvRecipientsPanel
        : public ConvRecipientsPanelView,
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
            ConvRecipientsPanel(Evas_Object *parent, App &app);
            virtual ~ConvRecipientsPanel();

            void read(Message &msg);
            void write(const Message &msg);
            bool isMms() const;
            void setListener(IConvRecipientsPanelListener *l);
            void update(const ThreadId &threadId);
            void update(const MsgAddressList &addressList);
            AppendItemStatus appendItem(const std::string &address, MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType);
            AppendItemStatus appendItem(const std::string &address, const std::string &dispName,
                              MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType);
            void execCmd(const AppControlComposeRef &cmd);

        private:
            // RecipientsPanelView:
            virtual void onItemAdded(ConvRecipientViewItem &item);
            virtual void onItemDeleted(ConvRecipientViewItem &item);
            virtual void onItemSelected(ConvRecipientViewItem &item);
            virtual void onItemClicked(ConvRecipientViewItem &item);
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
            IConvRecipientsPanelListener *m_pListener;
            ContactPicker m_Picker;
    };

    class IConvRecipientsPanelListener
    {
        public:
            virtual ~IConvRecipientsPanelListener() {}

            virtual void onKeyDown(ConvRecipientsPanel &panel, Evas_Event_Key_Down &ev) {};
            virtual void onEntryFocusChanged(ConvRecipientsPanel &panel) {};
            virtual void onItemAdded(ConvRecipientsPanel &panel, ConvRecipientItem &item) {};
            virtual void onItemDeleted(ConvRecipientsPanel &panel, ConvRecipientItem &item) {};
            virtual void onItemClicked(ConvRecipientsPanel &panel, ConvRecipientItem &item) {};
    };
}

#endif /* RecipientsPanel_h_ */
