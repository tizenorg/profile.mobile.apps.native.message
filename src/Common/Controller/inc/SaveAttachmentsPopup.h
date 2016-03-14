/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef SaveAttachmentsPopup_h_
#define SaveAttachmentsPopup_h_

#include "Popup.h"
#include "ListView.h"
#include "PopupAttachmentListItem.h"
#include "App.h"
#include "SelectAllListItem.h"
#include "MessageMms.h"

namespace Msg
{
    class SelectAllListItem;

    class SaveAttachmentsPopup
        : public PopupList
    {
        public:
            SaveAttachmentsPopup(App &app, const MessageMms &mms);
            virtual ~SaveAttachmentsPopup();

        private:
            void create(const MessageMms &mms);
            void showSelectAllItem();
            bool areAllItemsChecked() const;
            void updateSelectAllItem();
            void fillList(const MessageMms &mms);

            // Buttons
            void onCancelButtonClicked(Popup &popup, int buttonId);
            void onSaveButtonClicked(Popup &popup, int buttonId);
            bool isSaveButtonNeedToBeEnable();
            void disableSaveButton(bool value);

            // Save button clicked
            bool saveCheckedItems();
            void showSavingFailedPopup();

            // List View Listener
            virtual void onListItemChecked(ListItem &listItem);

            void checkSelectAllItem(SelectAllListItem &item);
            void checkItem(ListItem &item);

        private:
            App &m_App;
            Evas_Object *m_SaveButton;
    };
}

#endif /* SaveAttachmentsPopup_h_ */
