/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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
#include "PopupListItem.h"

namespace Msg
{
    /**
     * @brief A popup with list of available popup items.
     */
    class PopupList
        : public Popup
        , private IListViewListener
    {
        public:
            PopupList(Evas_Object *parent);
            PopupList(PopupManager &parent);
            virtual ~PopupList();

            /**
             * @brief Appends specified existing item to popup list.
             * @param[in] item an item created outside to be appended.
             */
            void appendItem(PopupListItem &item);

            /**
             * @brief Creates an item with a single string based on specified parameters and appends it to popup list.
             * @param[in] text a text displayed on popup list item.
             * @param[in] cb a callback raised by tap on this item.
             * @param[in] userData user data passed to item's on-press callback.
             */
            void appendItem(const std::string &text, PopupListItemPressedCb cb, void *userData);

            /**
             * Creates an item with a single string and a stored filepath based on specified parameters and appends it to popup list.
             * @param[in] text a text displayed on popup list item.
             * @param[in] path filepath.
             * @param[in] cb a callback raised by tap on this item.
             * @param[in] userData user data passed to item's on-press callback.
             */
            void appendItem(const std::string &text, const std::string &path, PopupListItemPressedCb cb, void *userData);

            /**
             * @brief Gets nested list-view.
             * @return list-view.
             */
            ListView &getListView();
            const ListView &getListView() const;

        private:
            virtual void onListItemSelected(ListItem &listItem);

            void create();
            Evas_Object *createList(Evas_Object *parent);

        private:
            ListView *m_pList;
    };
}

#endif /* PopupList_h_ */
