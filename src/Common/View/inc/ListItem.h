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

#ifndef ListItem_65152b60_4142_11e3_aa6e_0800200c9a66_h_
#define ListItem_65152b60_4142_11e3_aa6e_0800200c9a66_h_

#include "ViewItem.h"

#include <string>
#include <memory>
#include <vector>
#include <Elementary.h>

namespace Msg
{
    class ListView;
    class ListItemStyle;
    class ListItem;
    typedef std::shared_ptr<ListItemStyle> ListItemStyleRef;
    typedef std::vector<ListItem *> ListItemCollection;

    class ListItemStyle
    {
        public:
            ListItemStyle(const char *itemStyle,
                          const char *decorateItemStyle = nullptr,
                          const char *decorateAllItemStyle = nullptr);
            virtual ~ListItemStyle();

            static ListItemStyleRef create(const char *itemStyle,
                                           const char *decorateItemStyle = nullptr,
                                           const char *decorateAllItemStyle = nullptr);
        private:
            friend class ListView;
            friend class ListItem;

            void registerCallback();
            void unregisterCallback();
            void setStyle(const char *itemStyle, const char *decorateItemStyle, const char *decorateAllItemStyle);

        private:
            Elm_Genlist_Item_Class *m_pGenlistItemClass;
    };

    /**
     * @brief Wraps all basic genlist item operations.
     */
    class ListItem
        : public ViewItem
    {
        friend class ListItemStyle;
        friend class ListView;

        public:
            /**
             * @brief Creates instance of ListItem of specified genlist-item type.
             * @param[in] type genlist item-type.
             */
            ListItem(Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE);

            /**
             * @brief Creates instance of ListItem of specified genlist-item type with predefined style.
             * @param[in] itemStyle predefined item style.
             * @param[in] type genlist item-type.
             */
            ListItem(const ListItemStyleRef &itemStyle, Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE);
            virtual ~ListItem();

            // Base elm methods:
            /**
             * @brief Gets list-item style.
             * @return item style.
             */
            const ListItemStyleRef &getStyle() const;

            /**
             * @brief Sets specified style to list-item.
             * @param[in] style a style to be set.
             */
            void setStyle(ListItemStyleRef &style);

            /**
             * @brief Makes list-item selected(unselected).
             * @param[in] selected if true makes list-item selected, if false - unselects it.
             */
            void setSelected(bool selected);

            /**
             * @brief Checks whether list-item is selected or not.
             * @return true if list-item is selected, false otherwise.
             */
            bool getSelected() const;

            /**
             * @brief Sets the expanded state of an list-item.
             * @param[in] expand expanded state.
             */
            void setExpanded(bool expand);

            /**
             * @brief Update all the contents of an item.
             */
            void update();

            /**
             * @brief Updates specified parts in list-item.
             * @param[in] parts a parts to be updated. Used for globbing to match '*', '?', and '.'.
             * @param[in] type The type of item's part type.
             */
            void updateFields(const char *parts, Elm_Genlist_Item_Field_Type type);

            /**
             * @brief Gets a ListView instance that owns this list-item.
             * @return owner ListView.
             */
            ListView *getOwner() const;

            /**
             * @brief Gets type of nested genlist-item.
             * @return genlist item type.
             */
            Elm_Genlist_Item_Type getType() const;

            // Check(Select) methods:
            /**
             * @brief Changes checked state of list-item. Also item optionally can update full it's content.
             * @param[in] updateUi if true all list-item's content will be forced to update.
             */
            void changeCheckedState(bool updateUi);

            /**
             * @brief Updates list-item's checked state with state specified. Also item optionally can update full it's content.
             * @param[in] state a new checked-state to be applied.
             * @param[in] updateUi if true all list-item's content will be forced to update.
             */
            void setCheckedState(bool state, bool updateUi);

            /**
             * @brief Checks whether list- item is checked or not.
             * @return true if list-item is checked, false otherwise.
             */
            bool getCheckedState() const;

            /**
             * @brief Checks whether checkbox was created and put into specific list-item's part.
             * @return true if checkbox is created and available to manage, false otherwise.
             */
            bool isCheckable() const;

        protected:
            virtual std::string getText(ListItem &item, const char *part);
            virtual Evas_Object *getContent(ListItem &item, const char *part);
            virtual bool getState(ListItem &item, const char *part);
            virtual void onDelete(ListItem &item);
            virtual const char *getCheckPart(ListItem &item);

            virtual void onRealized(ListItem &item) {};
            virtual void onUnrealized(ListItem &item) {};

        private:
            virtual void onViewDestroyed();
            Evas_Object *addCheck();
            void onCheckChanged(Evas_Object *obj, void *eventInfo);

            static char * elm_gen_item_text_get_cb(void * data, Evas_Object * obj, const char * part);
            static Evas_Object * elm_gen_item_content_get_cb(void * data, Evas_Object * obj, const char * part);
            static Eina_Bool elm_gen_item_state_get_cb(void * data, Evas_Object * obj, const char * part);
            static void elm_gen_item_del_cb(void *data, Evas_Object *obj);

    private:
        ListView *m_pOwner;
        ListItemStyleRef m_ItemStyle;
        Elm_Genlist_Item_Type m_Type;
        bool m_Checked;
    };
}

#endif // ListItem_65152b60_4142_11e3_aa6e_0800200c9a66_h_
