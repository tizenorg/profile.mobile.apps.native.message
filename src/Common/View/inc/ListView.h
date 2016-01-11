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

    class ListView
        : public View
    {
        public:
            ListView(Evas_Object *parent);
            virtual ~ListView();

            void setListener(IListViewListener *listener);
            bool appendItem(ListItem &listItem, ListItem *parent = nullptr);
            bool prependItem(ListItem &listItem, ListItem *parent = nullptr);
            void deleteItem(ListItem &listItem);
            ListItemCollection getItems() const;
            ListItem *getFirstItem() const;
            ListItem *getLastItem() const;
            void showItem(ListItem &listItem, Elm_Genlist_Item_Scrollto_Type type);
            void setMultiSelection(bool value);
            bool getMultiSelection() const;
            void clear();
            bool isEmpty() const;
            void setMode(Elm_List_Mode mode);
            void setHomogeneous(bool isHomogeneous);
            ListItem *getSelectedItem() const;

            void setCheckMode(bool check);
            bool getCheckMode() const;
            void checkAllItems(bool check);

            template<typename T>
            std::vector<T*> getItems() const;

            void updateRealizedItems();

            void registerItemRealizedCallback();
            void registerItemExpandedCallback();
            void registerItemContractedCallback();

        private:
            typedef void (IListViewListener::*ListenerMethod)(ListItem &item, void *funcData);

        public:
            static void notifyListener(void *data, Evas_Object *obj, void *event_info, ListenerMethod method);

        private:
            void createListView(Evas_Object *parent);

            static void on_item_selected_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_drag_left_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_drag_right_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_realized_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_expanded_cb(void *data, Evas_Object *obj, void *event_info);
            static void on_contracted_cb(void *data, Evas_Object *obj, void *event_info);

        private:
            IListViewListener *m_pListener;
            bool m_CheckMode;
    };

    class IListViewListener
    {
    public:
        virtual ~IListViewListener() {};
        virtual void onListItemSelected(ListItem &listItem, void *funcData) {};
        virtual void onListItemDragLeft(ListItem &listItem, void *funcData) {};
        virtual void onListItemDragRight(ListItem &listItem, void *funcData) {};
        virtual void onListItemExpanded(ListItem &listItem, void *funcData) {};
        virtual void onListItemContracted(ListItem &listItem, void *funcData) {};
        virtual void onListItemRealized(ListItem &listItem, void *funcData) {};
        virtual void onListItemChecked(ListItem &listItem, void *funcData) {};
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
