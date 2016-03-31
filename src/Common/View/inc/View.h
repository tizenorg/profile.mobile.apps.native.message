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

#ifndef View_h_
#define View_h_

#include "BaseView.h"
#include "LangUtils.h"
#include "CallbackAssist.h"

#include <Evas.h>
#include <Elementary.h>

namespace Msg
{
    class View
        : public BaseView
    {
        public:
            View();

            void destroy();
            operator Evas_Object *() const;
            Evas_Object *getEo() const;
            bool isVisible() const;
            void show();
            void hide();
            void setVisibility(bool show);
            void move(Evas_Coord x, Evas_Coord y);
            void setSizeHintWeight(double x = EVAS_HINT_EXPAND, double y = EVAS_HINT_EXPAND);
            void setSizeHintAlign(double x = EVAS_HINT_FILL, double y = EVAS_HINT_FILL);
            void setScrollerContentMinLimit(Eina_Bool w, Eina_Bool h);
            void expand();
            static void expand(Evas_Object *obj);
            void setSizeHintMin(Evas_Coord w, Evas_Coord h);
            void setSizeHintMax(Evas_Coord w, Evas_Coord h);
            void getSizeHintMin(Evas_Coord *w, Evas_Coord *h);
            void getSizeHintMax(Evas_Coord *w, Evas_Coord *h);
            Evas_Object *setContent(Evas_Object *content, const char *part = nullptr, bool saveOldContent = false);
            Evas_Object *unsetContent(const char *part = nullptr);
            Evas_Object* getContent(const char *part = nullptr) const;
            void setFocusAllow(bool enable);
            bool getFocusAllow() const;
            void setFocus(bool focus);
            bool getFocus() const;
            void setTranslateble(bool translateble, const char *domain, const char *part = nullptr);
            void emitSignal(const char *emission, const char *source);
            void setData(const char *key, const void *data);
            void *getData(const char *key) const;
            void addEventCb(Evas_Callback_Type type, Evas_Object_Event_Cb func, const void *data);
            void addSmartCb(const char *event, Evas_Smart_Cb func, const void *data);
            void addSignalCb(const char *emission, const char *source, Edje_Signal_Cb func, void *data);

            template<typename T>
            static T staticCast(void *evasObj);
            template<typename T>
            static T reinterpretCast(void *evasObj);
            template<typename T>
            static T dynamicCast(void *evasObj);

            std::string getText(const char *part = nullptr) const;
            const char *getTextCStr(const char *part = nullptr) const;
            void setText(const char *text, const char *part = nullptr);
            void setText(const std::string &text, const char *part = nullptr);
            void setText(const TText &text, const char *part = nullptr);
            static void setText(Evas_Object *obj, const TText &text, const char *part = nullptr);

            static Evas_Object *addLayout(Evas_Object *parent, const std::string &edjePath, const std::string &group);

        protected:
            virtual ~View();
            virtual void onBeforeDelete(View &view) {};

            void setEventCb(Evas_Callback_Type type);
            void unsetEventCb(Evas_Callback_Type type);
            void setEo(Evas_Object *eo);

        private:
            View(View&) = delete;
            View& operator=(View&) = delete;
            Evas_Object_Event_Cb getCb(Evas_Callback_Type);
            void *getSmartData() const;
            static void *getSmartData(Evas_Object *obj);
            void setSmartData(const void *data);
            static void setSmartData(Evas_Object *obj, const void *data);

        private:
            static void on_free_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);
            static void on_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);

        private:
            Evas_Object *m_pEo;
    };

    std::string mrkupToUtf8(const char *str);
    std::string utf8ToMarkup(const char *str);
    std::string mrkupToUtf8(const std::string &str);
    std::string utf8ToMarkup(const std::string &str);

    inline View::operator Evas_Object *() const
    {
        return m_pEo;
    }

    inline Evas_Object *View::getEo() const
    {
        return m_pEo;
    }

    inline void View::destroy()
    {
        evas_object_del(m_pEo);
    }

    inline bool View::isVisible() const
    {
        return evas_object_visible_get(m_pEo);
    }

    inline void View::show()
    {
        evas_object_show(m_pEo);
    }

    inline void View::hide()
    {
        evas_object_hide(m_pEo);
    }

    inline void View::setVisibility(bool show)
    {
        if(show)
            View::show();
        else
            hide();
    }

    inline void View::setSizeHintWeight(double x, double y)
    {
        evas_object_size_hint_weight_set(m_pEo, x, y);
    }

    inline void View::setSizeHintAlign(double x, double y)
    {
        evas_object_size_hint_align_set(m_pEo, x, y);
    }

    inline void View::setScrollerContentMinLimit(Eina_Bool w, Eina_Bool h)
    {
        elm_scroller_content_min_limit(m_pEo, w, h);
    }

    inline void View::setSizeHintMin(Evas_Coord w, Evas_Coord h)
    {
        evas_object_size_hint_min_set(m_pEo, w, h);
    }

    inline void View::setSizeHintMax(Evas_Coord w, Evas_Coord h)
    {
        evas_object_size_hint_max_set(m_pEo, w, h);
    }

    inline void View::getSizeHintMin(Evas_Coord *w, Evas_Coord *h)
    {
        evas_object_size_hint_min_get(m_pEo, w, h);
    };

    inline void View::getSizeHintMax(Evas_Coord *w, Evas_Coord *h)
    {
        evas_object_size_hint_max_get(m_pEo, w, h);
    }

    inline void View::move(Evas_Coord x, Evas_Coord y)
    {
        evas_object_move(m_pEo, x, y);
    }

    inline Evas_Object* View::getContent(const char *part) const
    {
        return elm_object_part_content_get(m_pEo, part);
    }

    inline void View::setFocusAllow(bool enable)
    {
        elm_object_focus_allow_set(m_pEo, enable);
    }

    inline bool View::getFocusAllow() const
    {
        return elm_object_focus_allow_get(m_pEo);
    }

    inline void View::setFocus(bool focus)
    {
        return elm_object_focus_set(m_pEo, focus);
    }

    inline bool View::getFocus() const
    {
        return elm_object_focus_get(m_pEo);
    }

    inline void View::setTranslateble(bool translateble, const char *domain, const char *part)
    {
        elm_object_domain_part_text_translatable_set(m_pEo, part, domain, translateble);
    }

    inline void View::emitSignal(const char *emission, const char *source)
    {
        elm_object_signal_emit(m_pEo, emission, source);
    }

    inline void View::setText(const char *text, const char *part)
    {
        elm_object_part_text_set(m_pEo, part, text);
    }

    inline void View::setText(const std::string &text, const char *part)
    {
        setText(text.c_str(), part);
    }

    inline void View::setText(const TText &text, const char *part)
    {
        setText(m_pEo, text, part);
    }

    inline void View::setText(Evas_Object *obj, const TText &text, const char *part)
    {
        elm_object_domain_translatable_part_text_set(obj, part, text.getDomain(), text.getMsg());
    }

    inline std::string View::getText(const char *part) const
    {
        const char *text = getTextCStr(part);
        return text ? text : "";
    }

    inline const char *View::getTextCStr(const char *part) const
    {
        return elm_object_part_text_get(m_pEo, part);
    }

    inline void View::setData(const char *key, const void *data)
    {
        evas_object_data_set(m_pEo, key, data);
    }

    inline void *View::getData(const char *key) const
    {
        return evas_object_data_get(m_pEo, key);
    }

    inline void View::setSmartData(const void *data)
    {
        setSmartData(m_pEo, data);
    }

    inline void *View::getSmartData() const
    {
        return getSmartData(m_pEo);
    }

    inline void View::setSmartData(Evas_Object *obj, const void *data)
    {
        evas_object_smart_data_set(obj, (void*)data);
    }

    inline void *View::getSmartData(Evas_Object *obj)
    {
        return evas_object_smart_data_get(obj);
    }

    template<typename T>
    inline T View::staticCast(void *evasObj)
    {
        return static_cast<T>(getSmartData((Evas_Object*)evasObj));
    }

    template<typename T>
    inline T View::reinterpretCast(void *evasObj)
    {
        return reinterpret_cast<T>(getSmartData((Evas_Object*)evasObj));
    }

    template<typename T>
    inline T View::dynamicCast(void *evasObj)
    {
        return dynamic_cast<T>(staticCast<View*>(evasObj));
    }

    inline void View::addEventCb(Evas_Callback_Type type, Evas_Object_Event_Cb func, const void *data)
    {
        evas_object_event_callback_add(m_pEo, type, func, data);
    }

    inline void View::addSmartCb(const char *event, Evas_Smart_Cb func, const void *data)
    {
        evas_object_smart_callback_add(m_pEo, event, func, data);
    }

    inline void View::addSignalCb(const char *emission, const char *source, Edje_Signal_Cb func, void *data)
    {
        elm_object_signal_callback_add(m_pEo, emission, source, func, data);
    }

    inline Evas_Object *View::unsetContent(const char *part)
    {
        return elm_object_part_content_unset(m_pEo, part);
    }
}

#endif /* View_h_ */
