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

#include "View.h"
#include "Logger.h"
#include "PathUtils.h"

#include <cstddef>
#include <cassert>
#include <map>

using namespace Msg;

View::View()
    : m_pEo(nullptr)
{
}

View::~View()
{
}

Evas_Object_Event_Cb View::getCb(Evas_Callback_Type type)
{
    static std::map<Evas_Callback_Type, Evas_Object_Event_Cb> cbMap =
    {
        {EVAS_CALLBACK_FREE, on_free_cb},
        {EVAS_CALLBACK_DEL, on_del_cb}
    };

    auto it = cbMap.find(type);
    return it != cbMap.end() ? it->second : nullptr;
}

void View::setEventCb(Evas_Callback_Type type)
{
    Evas_Object_Event_Cb cb = getCb(type);
    if(cb)
    {
        evas_object_event_callback_add(m_pEo, type, cb, this);
    }
    else
    {
        MSG_LOG_ERROR("Not implemented");
        assert(false);
    }
}

void View::setEo(Evas_Object *eo)
{
    if(m_pEo)
    {
        // TODO: impl. reset EO if nedded
        MSG_LOG_ERROR("m_pEo not null");
        assert(false);
    }

    m_pEo = eo;
    if(eo)
    {
        setSmartData(this);
        setEventCb(EVAS_CALLBACK_FREE);
        setEventCb(EVAS_CALLBACK_DEL);
    }
}

void View::unsetEventCb(Evas_Callback_Type type)
{
    Evas_Object_Event_Cb cb = getCb(type);
    if(cb)
    {
        evas_object_event_callback_del(m_pEo, type, cb);
    }
    else
    {
        MSG_LOG_ERROR("Not implemented");
        assert(false);
    }
}

Evas_Object *View::setContent(Evas_Object *content, const char *part, bool saveOldContent)
{
    Evas_Object *oldContent = saveOldContent ? elm_object_part_content_unset(m_pEo, part) : nullptr;
    elm_object_part_content_set(m_pEo, part, content);
    return oldContent;
}

void View::on_free_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
    View *view = static_cast<View*>(data);
    if(view)
        view->onViewDestroyed();
}

void View::on_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
    View *view = static_cast<View*>(data);
    if(view)
        view->onBeforeDelete(*view);
}

void View::expand()
{
    expand(m_pEo);
}

void View::expand(Evas_Object *obj)
{
    evas_object_size_hint_weight_set(obj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(obj, EVAS_HINT_FILL, EVAS_HINT_FILL);
}

void View::processSignal(Evas_Object *obj)
{
    Evas_Object *edj = elm_layout_edje_get(obj);
    edje_object_message_signal_process(edj);
}

void View::emitSignal(Evas_Object *obj, const char *emission, const char *source, bool async)
{
    elm_object_signal_emit(obj, emission, source);
    if(!async)
        processSignal(obj);
}

Evas_Object *View::addLayout(Evas_Object *parent, const std::string &edjePath, const std::string &group)
{
    Evas_Object *layout = elm_layout_add(parent);
    std::string edjAbsPath = PathUtils::getResourcePath(edjePath);
    elm_layout_file_set(layout, edjAbsPath.c_str(), group.c_str());
    return layout;
}

std::string Msg::markupToUtf8(const char *str)
{
    if(!str)
        return std::string();

    std::string res;
    char *markup = elm_entry_markup_to_utf8(str);
    if(markup)
    {
        res = markup;
        free(markup);
    }

    return res;
}

std::string Msg::utf8ToMarkup(const char *str)
{
    if(!str)
        return std::string();

    std::string res;
    char *markup = elm_entry_utf8_to_markup(str);
    if(markup)
    {
        res = markup;
        free(markup);
    }

    return res;
}

std::string Msg::markupToUtf8(const std::string &str)
{
    return markupToUtf8(str.c_str());
}

std::string Msg::utf8ToMarkup(const std::string &str)
{
    return utf8ToMarkup(str.c_str());
}
