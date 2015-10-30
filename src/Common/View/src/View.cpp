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

#include <cstddef>
#include <cassert>

using namespace Msg;

View::View()
    : m_pEo(nullptr)
{

}

View::~View()
{
}

void View::destroy()
{
    evas_object_del(m_pEo);
}

void View::setEventCb(Evas_Callback_Type type)
{
    if(!m_pEo)
        return;

    switch(type)
    {
        case EVAS_CALLBACK_FREE:
            evas_object_event_callback_add(m_pEo, type, on_free_cb, this);
        break;

        default:
            MSG_LOG_ERROR("Not implemented");
            assert(false);
            break;
    }
}

void View::setEo(Evas_Object *eo)
{
    if(m_pEo)
    {
        MSG_LOG_ERROR("m_pEo not null");
        unsetEventCb(EVAS_CALLBACK_FREE);
    }

    m_pEo = eo;
    setEventCb(EVAS_CALLBACK_FREE);
}

void View::unsetEventCb(Evas_Callback_Type type)
{
    if(!m_pEo)
        return;

    switch(type)
    {
        case EVAS_CALLBACK_FREE:
            evas_object_event_callback_del(m_pEo, type, on_free_cb);
        break;

        default:
            MSG_LOG_ERROR("Not implemented");
            assert(false);
            break;
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

void View::expand()
{
    expand(m_pEo);
}

void View::expand(Evas_Object *obj)
{
    evas_object_size_hint_weight_set(obj, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(obj, EVAS_HINT_FILL, EVAS_HINT_FILL);
}
