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

#ifndef ConvListViewItem_H_
#define ConvListViewItem_H_

#include "ListItem.h"

namespace Msg
{
    class ConvListViewItem
        : public ListItem
    {
        public:
            enum ConvItemType
            {
                Sent,
                Received,
                Draft,
                Failed
            };

            ConvListViewItem(ConvItemType type);
            virtual ~ConvListViewItem();

        protected:
            virtual Evas_Object *getBubbleContent() = 0;
            virtual Evas_Object *getThumbnail() = 0;
            virtual Evas_Object *getProgress() = 0;
            virtual std::string getText() = 0;
            virtual std::string getTime() = 0;

            virtual void onEditButtonClicked(Evas_Object *obj, void *event_info) = 0;
            virtual void onFailedButtonClicked(Evas_Object *obj, void *event_info) = 0;

        protected:
            Evas_Object *createProgress();
            void updateProgressField();
            void updateItemType(ConvItemType type);

        private:
            Evas_Object *createButton(bool isEnabled, ConvItemType type);
            virtual std::string getText(ListItem &item, const char *part);
            virtual Evas_Object *getContent(ListItem &item, const char *part);
            virtual const char *getCheckPart(ListItem &item);
    };
}

#endif /* ConvListViewItem_H_ */
