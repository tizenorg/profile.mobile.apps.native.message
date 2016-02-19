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

#include "SmilTextItemView.h"
#include "Logger.h"

#include <efl_extension.h>

using namespace Msg;

SmilTextItemView::SmilTextItemView(Evas_Object *parent, const std::string &text)
{
    setEo(addLayout(parent, SMIL_ITEM_EDJ_PATH, "smil_text_item"));
    Evas_Object *entry = createEntry(getEo(), text);
    setContent(entry, "swallow.content");
}

SmilTextItemView::~SmilTextItemView()
{
}

Evas_Object *SmilTextItemView::createEntry(Evas_Object *parent, const std::string &text)
{
    Evas_Object *entry = elm_entry_add(parent);
    evas_object_show(entry);

    char *markupText = elm_entry_utf8_to_markup(text.c_str());
    if(markupText)
    {
        elm_entry_entry_set(entry, markupText);
        free(markupText);
    }

    elm_entry_line_wrap_set(entry, ELM_WRAP_MIXED);
    elm_entry_editable_set(entry, false);
    eext_entry_selection_back_event_allow_set(entry, true);
    return entry;
}
