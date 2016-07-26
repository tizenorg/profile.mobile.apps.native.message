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
    /**
     * @brief Wraps all basic Elm_Object_Item operations and manages automatic self-removal by delete-callback.
     */
    class ViewItem
        : public BaseView
    {
        public:
            ViewItem();
            ViewItem(ViewItem&) = delete;
            ViewItem& operator=(const ViewItem&) = delete;

            operator Elm_Object_Item *() const;

            /**
             * @brief Destroys view-item.
             */
            virtual void destroy();

            /**
             * @brief Sets specified content into specified part.
             * @param[in] content a content to set into view-item.
             * @param[in] part a part content should be placed to. If part is nullptr the default part is used.
             * @param[in] saveOldContent a flag that checks whether old view's content should be returned or not.
             * @return if saveOldContent is true returns old content set into view before, otherwise returns nullptr.
             */
            Evas_Object *setContent(Evas_Object *content, const char *part = nullptr, bool saveOldContent = false);

            /**
             * @brief Gets view-items's content.
             * @param[in] part a content-part. If part is nullptr the default part is used.
             * @return view's content in case of success, nullptr in case of errors.
             */
            Evas_Object *getContent(const char *part = nullptr) const;

            /**
             * @brief Sends a signal to edje-object.
             * @param[in] emission The signal's name.
             * @param[in] source The signal's source.
             */
            void emitSignal(const char *signal, const char *source = "elm");

            /**
             * @brief Gets item-data.
             * @return item-data.
             */
            void *getData() const;

            /**
             * @brief Sets item-data.
             * @param[in] data item-data.
             */
            void setData(void *data);

            /**
             * @brief Gets Elm_Object_Item current view-item is wrapping.
             * @return wrapped object-item.
             */
            Elm_Object_Item *getElmObjItem() const;

            /**
             * @brief Get the widget object's handle which contains a given view-item.
             */
            Evas_Object *getWidget() const;

            /**
             * @brief Disables(enables) view-item.
             * @param[in] val if true disables view-item, otherwise enables it.
             */
            void disabled(bool val);

            /**
             * @brief Checks whether view-item is disabled or not.
             * @return true if view-item is disabled, otherwise false.
             */
            bool isDisabled() const;

            /**
             * @brief Mark the part text to be translatable or not.
             * @param[in] translatable @c true, the part text will be translated internally. @c false, otherwise.
             * @param[in] domain The translation domain to use.
             * @param[in] part The part name of the translatable text, if part is nullptr the default part is used.
             */
            void setTranslatable(bool translatable, const char *domain, const char *part = nullptr);

            /**
             * @brief Gets text placed in specified part.
             * @param[in] part a part to get text from. If part is nullptr the default part is used.
             * @return a text part contains. If part contains no text returns empty string.
             */
            std::string getText(const char *part = nullptr) const;

            /**
             * @brief a C-style implementation of getText().
             */
            const char *getTextCStr(const char *part = nullptr) const;

            /**
             * @brief Sets text into specified part.
             * @param[in] text a text to be set in specified part
             * @param[in] part a part to get text from. If part is nullptr the default part is used.
             */
            void setText(const char *text, const char *part = nullptr);

            /**
             * @brief Sets text into specified part.
             * @param[in] text a text to be set in specified part
             * @param[in] part a part to get text from. If part is nullptr the default part is used.
             */
            void setText(const std::string &text, const char *part = nullptr);

            /**
             * @brief Sets IDS of translatable string into specified part.
             * @param[in] text a translatable text to be set in specified part.
             * @param[in] part a part to get text from. If part is nullptr the default part is used.
             */
            void setText(const TText &text, const char *part = nullptr);
            static void setText(Elm_Object_Item *it, const TText &text, const char *part = nullptr);

            template<typename T>
            static T staticCast(void *elmObjectItem);
            template<typename T>
            static T reinterpretCast(void *elmObjectItem);
            template<typename T>
            static T dynamicCast(void *elmObjectItem);

        protected:
            virtual ~ViewItem();
            virtual void onAttached(ViewItem &item) {};
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

    inline void ViewItem::setTranslatable(bool translatable, const char *domain, const char *part)
    {
        elm_object_item_domain_part_text_translatable_set(m_pItem, part, domain, translatable);
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

    template<typename T>
    inline T ViewItem::staticCast(void *elmObjectItem)
    {
        return static_cast<T>(elm_object_item_data_get((Elm_Object_Item*)elmObjectItem));
    }

    template<typename T>
    inline T ViewItem::reinterpretCast(void *elmObjectItem)
    {
        return reinterpret_cast<T>(elm_object_item_data_get((Elm_Object_Item*)elmObjectItem));
    }

    template<typename T>
    inline T ViewItem::dynamicCast(void *elmObjectItem)
    {
        return dynamic_cast<T>(staticCast<ViewItem*>(elmObjectItem));
    }
}

#endif /* ViewItem_h_ */
