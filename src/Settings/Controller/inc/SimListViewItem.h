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

namespace Msg
{
    class SimListViewItem
        : public ListItem
    {
        public:
            SimListViewItem(Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE);
            virtual ~SimListViewItem();
        protected:
            virtual std::string getName() const;
            virtual std::string getMessage() const;
            virtual std::string getTime() const;

        private:
            virtual const char *getCheckPart(ListItem &item);
            // ListItem:
            virtual std::string getText(ListItem &item, const char *part);
    };
}

#endif // SimListViewItem_h_
