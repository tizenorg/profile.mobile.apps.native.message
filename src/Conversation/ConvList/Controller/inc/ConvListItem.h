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

#ifndef ConvListItem_H_
#define ConvListItem_H_

#include "ConvListViewItem.h"
#include "MsgTypes.h"
#include "BubbleView.h"
#include "Message.h"
#include "ContextPopup.h"
#include "App.h"

namespace Msg
{
    class MsgConversationItem;

    class ConvListItem
        : public ConvListViewItem
    {
        public:
            /**
             * @brief Creates item for Conversation list
             * @param[in] item MsgConversationItem model
             */
            ConvListItem(MsgConversationItem &item, App &app);
            virtual ~ConvListItem();

            /**
             * @brief Returns MsgId related to this ConvListItem
             */
            MsgId getMsgId() const;
            void showPopup();

        protected:
            virtual Evas_Object *getBubble();
            virtual std::string getText();
            virtual std::string getTime();

        private:
            // Create Popup when message is clicked
            void showMainCtxPopup();
            void showDraftCtxPopup();
            void onDeleteItemPressed(ContextPopupItem &item);
            void onCopyTextItemPressed(ContextPopupItem &item);
            void onForwardItemPressed(ContextPopupItem &item);
            void onResendItemPressed(ContextPopupItem &item);
            void onSlideShowItemPressed(ContextPopupItem &item);
            void onEditItemPressed(ContextPopupItem &item);
            void onSaveAttachmentsItemPressed(ContextPopupItem &item);
            void onCopyToSimCardItemPressed(ContextPopupItem &item);
            void onViewDetailsItemPressed(ContextPopupItem &item);

        private:
            App &m_App;
            MsgId m_MsgId;
            std::string m_MessageText;
            bool m_IsDraft;
            Message::Status m_Status;
            Message::Type m_Type;
    };
}


#endif /* ConvListItem_H_ */
