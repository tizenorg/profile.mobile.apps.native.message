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

#ifndef SettingsListItem_h_
#define SettingsListItem_h_

#include "SettingsListViewItem.h"

namespace Msg
{
    class Settings;

    class SettingsListItem
        : public SettingsListViewItem
    {
        public:
            typedef void (Settings::*MethodHandler)(SettingsListItem &item);

        public:
            inline SettingsListItem(const ListItemStyleRef &style, MethodHandler method);
            virtual ~SettingsListItem() {};

            inline MethodHandler getHandler() const;

        private:
            MethodHandler m_Handler;
    };

    inline SettingsListItem::SettingsListItem(const ListItemStyleRef &style, MethodHandler method)
        : SettingsListViewItem(style)
        , m_Handler(method)
    {
    }

    inline SettingsListItem::MethodHandler SettingsListItem::getHandler() const
    {
        return m_Handler;
    }
}

#endif // SettingsListItem_h_
