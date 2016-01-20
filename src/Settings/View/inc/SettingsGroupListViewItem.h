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

#ifndef SettingsGroupListViewItem_h
#define SettingsGroupListViewItem_h

#include "ListItem.h"
#include <string>

namespace Msg
{
    class SettingsGroupListViewItem
        : public ListItem
    {
        public:
            SettingsGroupListViewItem(const std::string &title);
            virtual ~SettingsGroupListViewItem();

        private:
            // ListItem:
            virtual std::string getText(ListItem &item, const char *part);

        private:
            const std::string m_Title;
    };
}

#endif // SettingsGroupListViewItem_h
