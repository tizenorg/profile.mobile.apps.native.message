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

#ifndef SimListViewItem_h_
#define SimListViewItem_h_

#include "ListItem.h"
#include <string>
#include "MessageSMS.h"

namespace Msg
{
    /**
     * Visual representation of an element in "Messages on SIM" list.
     */
    class SimListViewItem
        : public ListItem
    {
        public:
            /**
             * @brief Creates element of "Messages on SIM" list based on message-service structure and genlist item type passed from outside.
             * @param[in] msg a reference to message-service structure wrapped into class from Common module.
             * @param[in] type type of genlist-item.
             */
            SimListViewItem(const MessageSMS &msg, Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE);
            virtual ~SimListViewItem();

            /**
             * @brief gets message-id
             * @return id of message associated with current list-element.
             */
            MsgId getMsgId() const;

        protected:
            virtual std::string getName() const;
            virtual std::string getMessage() const;

        private:
            void updateModel(const MessageSMS &msg);
            virtual const char *getCheckPart(ListItem &item);
            // ListItem:
            virtual std::string getText(ListItem &item, const char *part);

        private:
            std::string m_Name;
            std::string m_MessageText;
            std::string m_Time;
            MsgId m_MsgId;
    };
}

#endif // SimListViewItem_h_
