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

#include <Evas.h>
#include <Elementary.h>

namespace Msg
{
    class View
        : public BaseView
    {
        public:
            View();
            virtual ~View();
            virtual void destroy();

            inline operator Evas_Object *() const;
            inline Evas_Object *getEo() const;
            inline void show();
            inline void hide();
            inline void move(Evas_Coord x, Evas_Coord y);
            inline void setSizeHintWeight(double x = EVAS_HINT_EXPAND, double y = EVAS_HINT_EXPAND);
            inline void setSizeHintAlign(double x = EVAS_HINT_FILL, double y = EVAS_HINT_FILL);
            void expand();
            static void expand(Evas_Object *obj);
            inline void setSizeHintMin(Evas_Coord w, Evas_Coord h);
            inline void setSizeHintMax(Evas_Coord w, Evas_Coord h);
            Evas_Object *setContent(Evas_Object *content, const char *part = nullptr, bool saveOldContent = false);
            inline Evas_Object* getContent(const char *part = nullptr) const;
            inline void setFocusAllow(bool enable);
            inline bool getFocusAllow() const;
            inline void setFocus(bool focus);
            inline bool getFocus() const;
            inline void setTranslateble(bool translateble, const char *domain, const char *part = nullptr);
            inline void emitSignal(const char *emission, const char *source);
            inline void setData(const char *key, const void *data);
            inline void *getData(const char *key) const;

            inline std::string getText(const char *part = nullptr) const;
            inline const char *getTextCStr(const char *part = nullptr) const;
            inline void setText(const char *text, const char *part = nullptr);
            inline void setText(const std::string &text, const char *part = nullptr);
            inline void setText(const TText &text, const char *part = nullptr);
            inline static void setText(Evas_Object *obj, const TText &text, const char *part = nullptr);

        protected:
            virtual void onBeforeDelete(View &view) {};

            void setEventCb(Evas_Callback_Type type);
            void unsetEventCb(Evas_Callback_Type type);
            void setEo(Evas_Object *eo);

        private:
            View(View&) = delete;
            View& operator=(View&) = delete;
            Evas_Object_Event_Cb getCb(Evas_Callback_Type);

        private:
            static void on_free_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);
            static void on_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);

        private:
            Evas_Object *m_pEo;
    };

    inline View::operator Evas_Object *() const
    {
        return m_pEo;
    }

    inline Evas_Object *View::getEo() const
    {
        return m_pEo;
    }

    inline void View::show()
    {
        evas_object_show(m_pEo);
    }

    inline void View::hide()
    {
        evas_object_hide(m_pEo);
    }

    inline void View::setSizeHintWeight(double x, double y)
    {
        evas_object_size_hint_weight_set(m_pEo, x, y);
    }

    inline void View::setSizeHintAlign(double x, double y)
    {
        evas_object_size_hint_align_set(m_pEo, x, y);
    }

    inline void View::setSizeHintMin(Evas_Coord w, Evas_Coord h)
    {
        evas_object_size_hint_min_set(m_pEo, w, h);
    }

    inline void View::setSizeHintMax(Evas_Coord w, Evas_Coord h)
    {
        evas_object_size_hint_max_set(m_pEo, w, h);
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
}

#endif /* View_h_ */
