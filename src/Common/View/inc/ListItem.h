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

            void registerCallback();
            void unregisterCallback();
            void setStyle(const char *itemStyle, const char *decorateItemStyle, const char *decorateAllItemStyle);

        private:
            Elm_Genlist_Item_Class *m_pGenlistItemClass;
    };

    class ListItem
        : public ViewItem
    {
        friend class ListItemStyle;
        friend class ListView;

        public:
            ListItem(Elm_Genlist_Item_Type type = ELM_GENLIST_ITEM_NONE);
            ListItem(const ListItemStyleRef &itemStyle, Elm_Genlist_Item_Type type);
            virtual ~ListItem();

            // Base elm methods:
            const ListItemStyleRef &getStyle() const;
            void setStyle(ListItemStyleRef &style);
            void setSelected(bool selected);
            bool getSelected() const;
            void setExpanded(bool expand);
            void update();
            void updateFields(const char *parts, Elm_Genlist_Item_Field_Type type);;
            ListView *getOwner() const;
            Elm_Genlist_Item_Type getType() const;
            int getUserType() const;

            // Check(Select) methods:
            void changeCheckedState(bool updateUi);
            void setCheckedState(bool state, bool updateUi);
            bool getCheckedState() const;
            bool isCheckable() const;

        protected:
            // TODO: remove ListItem prefix
            virtual std::string getListItemText(ListItem &item, const char *part);
            virtual Evas_Object *getListItemContent(ListItem &item, const char *part);
            virtual bool getListItemState(ListItem &item, const char *part);
            virtual void onListItemDelete(ListItem &item);
            virtual const char *getCheckPart(ListItem &item);

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
