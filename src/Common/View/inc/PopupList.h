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

#ifndef PopupList_h_
#define PopupList_h_

#include "Popup.h"
#include "ListView.h"

namespace Msg
{
    class PopupList;
    class PopupListItem;
    typedef void (*PopupListItemPressedCb)(PopupListItem &item, void *userData);

    #define POPUPLIST_ITEM_PRESSED_CB(ClassName, method) [](PopupListItem &item, void *userData) \
    {                                                                                            \
        static_cast<ClassName*>(userData)->method(item);                                         \
    }

    class PopupListItem
        : public ListItem
    {
        friend class PopupList;
        public:
            PopupListItem(PopupList &parent, const std::string &text, PopupListItemPressedCb cb, void *userData);
            PopupList &getParent();

        private:
            virtual std::string getText(ListItem &item, const char *part);

        private:
            PopupList &m_Parent;
            std::string m_Text;
            PopupListItemPressedCb m_Cb;
            void *m_pUserData;
    };

    class PopupList
        : public Popup
        , private IListViewListener
    {
        public:
            PopupList(Evas_Object *parent);
            PopupList(PopupManager &parent);
            virtual ~PopupList();

            void appendItem(const std::string &text, PopupListItemPressedCb cb, void *userData);
            ListView &getListView();

        private:
            virtual void onListItemSelected(ListItem &listItem);

            void create();
            Evas_Object *createList(Evas_Object *parent);

        private:
            ListView *m_pList;
    };
}

#endif /* PopupList_h_ */
