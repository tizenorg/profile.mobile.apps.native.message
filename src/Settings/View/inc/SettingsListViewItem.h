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

#ifndef SettingsOnOffListViewItem_h_
#define SettingsOnOffListViewItem_h_

#include "ListItem.h"

namespace Msg
{
    class ISettingsListViewItemListener;

    class SettingsListViewItem
        : public ListItem
    {
        public:
            static ListItemStyleRef multiLineStyle;
            static ListItemStyleRef multiLineIconStyle;
            static ListItemStyleRef oneLineIconStyle;

        public:
            SettingsListViewItem(const ListItemStyleRef &style);
            virtual ~SettingsListViewItem();

            void showCheckButton(bool show);
            void disabledCheckButton(bool disabled);
            void setCheckButtonState(bool state);
            bool getCheckButtonState() const;
            void changeCheckButtonState();
            void setMainText(const std::string &text);
            void setSubText(const std::string &text);
            void setListener(ISettingsListViewItemListener *listener);

        private:
            // ListItem:
            virtual std::string getListItemText(ListItem &item, const char *part);
            virtual Evas_Object *getListItemContent(ListItem &item, const char *part);

            void onCheckButtonChanged(Evas_Object *obj, void *eventInfo);

            Evas_Object *createCheckButton(Evas_Object *parent);

        private:
            std::string m_MainText;
            std::string m_SubText;
            bool m_ShowCheckButton;
            Eina_Bool m_CheckButtonState;
            bool m_DisabledCheckButton;
            ISettingsListViewItemListener *m_pListener;
    };

    class ISettingsListViewItemListener
    {
        public:
            virtual ~ISettingsListViewItemListener(){}
            virtual void onCheckButtonChanged(SettingsListViewItem &item) {};
    };
}

#endif // SettingsOnOffListViewItem_h_
