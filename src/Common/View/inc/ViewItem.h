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

#ifndef ViewItem_h_
#define ViewItem_h_

#include "BaseView.h"
#include "LangUtils.h"
#include "Logger.h"

#include <Elementary.h>

namespace Msg
{
    class ViewItem
        : public BaseView
    {
        public:
            ViewItem();
            virtual ~ViewItem();

            inline operator Elm_Object_Item *() const;
            virtual void destroy();
            Evas_Object *setContent(Evas_Object *content, const char *part = nullptr, bool saveOldContent = false);
            Evas_Object *getContent(const char *part = nullptr) const;
            void emitSignal(const char *signal, const char *source = "elm");
            inline void *getData() const;
            inline void setData(void *data);
            inline Elm_Object_Item *getElmObjItem() const;
            inline Evas_Object *getWidget() const;
            inline void disabled(bool val);
            inline bool isDisabled() const;
            inline void setTranslateble(bool translateble, const char *domain, const char *part = nullptr);

            inline std::string getText(const char *part = nullptr) const;
            inline const char *getTextCStr(const char *part = nullptr) const;
            inline void setText(const char *text, const char *part = nullptr);
            inline void setText(const std::string &text, const char *part = nullptr);
            inline void setText(const TText &text, const char *part = nullptr);
            inline static void setText(Elm_Object_Item *it, const TText &text, const char *part = nullptr);

        protected:
            virtual void onViewItemCreated() {};
            void setElmObjItem(Elm_Object_Item *item);

        private:
            static void on_delete_cb(void *data, Evas_Object *obj, void *event_info);

        private:
            Elm_Object_Item *m_pItem;
    };

    inline ViewItem::operator Elm_Object_Item *() const
    {
        return m_pItem;
    }

    inline Elm_Object_Item *ViewItem::getElmObjItem() const
    {
        return m_pItem;
    }

    inline Evas_Object *ViewItem::getWidget() const
    {
        return elm_object_item_widget_get(m_pItem);
    }

    inline void *ViewItem::getData() const
    {
        return elm_object_item_data_get(m_pItem);
    }

    inline void ViewItem::setData(void *data)
    {
        elm_object_item_data_set(m_pItem, data);
    }

    inline void ViewItem::disabled(bool val)
    {
        elm_object_item_disabled_set(m_pItem, val);
    }

    inline bool ViewItem::isDisabled() const
    {
        return elm_object_item_disabled_get(m_pItem);
    }

    inline void ViewItem::setText(const char *text, const char *part)
    {
        elm_object_item_part_text_set(m_pItem, part, text);
    }

    inline void ViewItem::setTranslateble(bool translateble, const char *domain, const char *part)
    {
        elm_object_item_domain_part_text_translatable_set(m_pItem, part, domain, translateble);
    }

    inline void ViewItem::setText(const std::string &text, const char *part)
    {
        setText(text.c_str(), part);
    }

    inline void ViewItem::setText(const TText &text, const char *part)
    {
        setText(m_pItem, text, part);
    }

    inline void ViewItem::setText(Elm_Object_Item *it, const TText &text, const char *part)
    {
        elm_object_item_domain_translatable_part_text_set(it, part, text.getDomain(), text.getMsg());
    }

    inline std::string ViewItem::getText(const char *part) const
    {
        const char *text = getTextCStr(part);
        return text ? text : "";
    }

    inline const char *ViewItem::getTextCStr(const char *part) const
    {
        return elm_object_item_part_text_get(m_pItem, part);
    }

}

#endif /* ViewItem_h_ */
