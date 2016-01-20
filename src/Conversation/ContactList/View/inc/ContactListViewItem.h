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

#ifndef ContactListItemView_h_
#define ContactListItemView_h_

#include "ListItem.h"
#include <string>

namespace Msg
{
    class ContactListViewItem
        : public ListItem
    {
        public:
            ContactListViewItem();
            virtual ~ContactListViewItem();

        protected:
            static ListItemStyleRef logStyle;
            static ListItemStyleRef nameOrEmailStyle;

            virtual std::string getSubText() const = 0;
            virtual std::string getMainText() const = 0;
            virtual Evas_Object *getThumbnail() const = 0;

        private:
            virtual std::string getText(ListItem &item, const char *part);
            virtual Evas_Object *getContent(ListItem &item, const char *part);
    };
}

#endif // ContactListItemView_h_
