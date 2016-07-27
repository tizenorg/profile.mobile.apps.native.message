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

#ifndef ListView_15152b60_4142_11b3_aa6e_080a200c9a62_h_
#define ListView_15152b60_4142_11b3_aa6e_080a200c9a62_h_

#include "ListItem.h"
#include "View.h"

#include <Evas.h>

namespace Msg
{
    class IListViewListener;

    /**
     * @brief Wraps all basic genlist operations.
     */
    class ListView
        : public View
    {
        public:
            /**@brief Function of comparing two list-items, used for sorted item insertion.*/
            typedef int (*CmpFunc)(const ListItem &item1, const ListItem &item2);

        public:
            /**
             * @brief A constructor of ListView object based on outside parent object.
             * @param[in] parent an object responsible for automatic removing of ListView instance.
             */
            ListView(Evas_Object *parent);
            virtual ~ListView();

            /**
             * @brief Sets subscriber that will be notified on list-view events.
             * @param[in] listener a listener.
             */
            void setListener(IListViewListener *listener);

            /**
             * @brief Appends specified list-view item into list-view.
             * @param[in] listItem an item to be appended.
             * @param[in] parent a parent item.
             * @return true in case of success, false otherwise.
             */
            bool appendItem(ListItem &listItem, ListItem *parent = nullptr);

            /**
             * @brief Prepends specified list-view item into list-view.
             * @param[in] listItem an item to be prepended.
             * @param[in] parent a parent item.
             * @return true in case of success, false otherwise.
             */
            bool prependItem(ListItem &listItem, ListItem *parent = nullptr);

            /**
             * @brief Performs sorted item insertion.
             * @param[in] listItem an item to be inserted.
             * @param[in] parent a parent item.
             * @return true in case of success, false otherwise.
             */
            bool sortedInsertItem(ListItem &listItem, ListItem *parent = nullptr);

            /**
             * @brief Sets item compare function to make work of sortedInsertItem() correct.
             * @param[in] fn items compare function.
             */
            void setCmpFunc(CmpFunc fn);

            /**
             * @brief Deletes item from list-view.
             * @param[in, out] listItem item to be deleted.
             */
            void deleteItem(ListItem &listItem);

            /**
             * @brief Gets all list-view items.
             * @return list-view items.
             */
            ListItemCollection getItems() const;

            /**
             * @brief Gets first list-view item.
             * @return first list-view item.
             */
            ListItem *getFirstItem() const;

            /**
             * @brief Gets last list-view item.
             * @return last list-view item.
             */
            ListItem *getLastItem() const;

            /**
             * @brief Shows list-view item. This causes list-view to jump to the given item @c it and show it (by jumping
             * to that position), if it is not fully visible.
             * @param[in] listItem an item to be shown.
             * @param[in] type The position to bring in, the given item to.
             */
            void showItem(ListItem &listItem, Elm_Genlist_Item_Scrollto_Type type);

            /**
             * @brief Enables(disables) multiselection.
             * @param[in] value if true enables multiselection, otherwise disables it.
             */
            void setMultiSelection(bool value);

            /**
             * @brief Checks whether multiselection is turned on or not.
             * @return true if multiselection is enabled, otherwise false.
             */
            bool getMultiSelection() const;

            /**
             * @brief Removes all list-items.
             */
            void clear();

            /**
             * @brief Checks ehether list-view contains at least one item or not.
             * @return true if list-view has no items, otherwise false.
             */
            bool isEmpty() const;

            /**
             * @brief Passes mode to nested genlist.
             * @param[in] mode a mode to be applied.
             */
            void setMode(Elm_List_Mode mode);

            /**
             * @brief Enable(disable) genlist homogeneous mode.
             * @param[in] isHomogeneous if true torns on homogeneous mode, otherwise turns it off.
             */
            void setHomogeneous(bool isHomogeneous);

            /**
             * @brief Gets selected view-item.
             * @return selected list-view item or nullptr if no item is selected.
             */
            ListItem *getSelectedItem() const;

            /**
             * @brief Gets number of items.
             * @return number of items.
             */
            unsigned getItemsCount() const;

            /**
             * @brief Gets list-view item next to specified one.
             * @param[in] item an item in genlist.
             * @return next item to the specified one. If there's no next item nullptr is returned.
             */
            ListItem *getNextItem(ListItem &item) const;

            /**
             * @brief Gets list-view item previous to specified one.
             * @param[in] item  an item in genlist.
             * @return next item to the specified one. If there's no previous item nullptr is returned.
             */
            ListItem *getPrevItem(ListItem &item) const;

            /**
             * @brief Demotes specified list-view item to the end of the list.
             * @param[in] item an item to be demoted.
             */
            void demoteItem(ListItem &item);

            /**
             * @brief Enables(disables selection mode). Makes all list-view items to show(hide) a check-box.
             * @param[in] check if true makes all items to show checkbox, otherwise make them to hide it.
             */
            void setCheckMode(bool check);

            /**
             * @brief Checks wheather selection mode is enabled or not.
             * @return true if selection mode is enabled, false otherwise.
             */
            bool getCheckMode() const;

            /**
             * @brief Checks(unchecks) all list-view items in selection mode.
             * @param[in] check if true all items will be checked, if false - unchecks all items.
             */
            void checkAllItems(bool check);

            template<typename T>
            std::vector<T*> getItems() const;

            /**
             * @brief Updates the contents of all realized items.
             */
            void updateRealizedItems();

        private:
            typedef void (IListViewListener::*ListenerMethod)(ListItem &item);

        public:
            static void notifyListener(void *data, Evas_Object *obj, void *event_info, ListenerMethod method);

        private:
            void createListView(Evas_Object *parent);

            static void on_item_selected_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_realized_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_unrealized_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_longpressed_cb(void *data, Evas_Object *obj, void *event_info);

        private:
            IListViewListener *m_pListener;
            bool m_CheckMode;
            CmpFunc m_CmpFunc;
    };

    /**
     * @brief A listener-interface aimed to make subscribers to get notified about genlist events.
     */
    class IListViewListener
    {
    public:
        virtual ~IListViewListener() {};

        /**
         * @brief Notification about insertion of new item into genlist.
         * @param[in, out] listItem inserted item
         */
        virtual void onListItemSelected(ListItem &listItem) {};

        /**
         * @brief Notification about genlist-item longpress event.
         * @param[in, out] listItem lingpressed item
         */
        virtual void onListItemLongPressed(ListItem &listItem) {};

        /**
         * @brief Notification occured after changing check-state of check-box placed on genlist-item.
         * @param[in, out] listItem checked item
         */
        virtual void onListItemChecked(ListItem &listItem) {};
    };

    template<typename T>
    std::vector<T*> ListView::getItems() const
    {
        std::vector<T*> list;
        Elm_Object_Item *elmItem = elm_genlist_first_item_get(getEo());
        while(elmItem)
        {
            void *data = elm_object_item_data_get(elmItem);
            if(data)
            {
                T *listItem = dynamic_cast<T*>(static_cast<ListItem*>(data));
                if(listItem)
                    list.push_back(listItem);
            }
            elmItem = elm_genlist_item_next_get(elmItem);
        }

        return list;
    }
}

#endif // ListView_15152b60_4142_11b3_aa6e_080a200c9a62_h_
