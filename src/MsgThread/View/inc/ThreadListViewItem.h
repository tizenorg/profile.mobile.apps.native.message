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

#ifndef ThreadListViewItem_h_
#define ThreadListViewItem_h_

#include "ListItem.h"
#include <string>

namespace Msg
{
    class ThreadListViewItem
        : public ListItem
    {
        public:
            enum State
            {
                NormalState,
                StatusState,
                IconState
            };

        public:
            ThreadListViewItem(Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE);
            virtual ~ThreadListViewItem();

            void setState(State state, bool updateUi);
            State getState() const;

        protected:
            virtual std::string getName() = 0;
            virtual std::string getMessage() = 0;
            virtual std::string getTime() = 0;
            virtual Evas_Object *getThumbnail() = 0;
            virtual std::string getStatus();
            virtual Evas_Object *getIcon();

            using ListItem::getState;
            Evas_Object *makeUnreadIcon(const std::string &text) const;
            std::string decorateNameText(const std::string &text) const;
            std::string decorateMessageText(const std::string &text) const;
            std::string decorateTimeText(const std::string &text) const;
            std::string decorateUnreadText(const std::string &text) const;
            std::string decorateFailedText(const std::string &text) const;
            std::string decorateSendingText(const std::string &text) const;
            std::string decorateDraftText(const std::string &text) const;

        private:
            virtual std::string getText(ListItem &item, const char *part);
            virtual Evas_Object *getContent(ListItem &item, const char *part);
            virtual const char *getCheckPart(ListItem &item);

        private:
            State m_State;
    };
}

#endif // ThreadListViewItem_h_
