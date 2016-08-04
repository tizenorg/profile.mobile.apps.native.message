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

#include "ConvListViewItem.h"
#include "CallbackAssist.h"
#include "ListView.h"
#include "Resource.h"

using namespace Msg;

namespace
{
    //TODO: Implement different styles
    ListItemStyleRef sentStyle = ListItemStyle::create("sentbubble");
    ListItemStyleRef receivedStyle = ListItemStyle::create("receivebubble");
    ListItemStyleRef draftStyle = ListItemStyle::create("draft");
    ListItemStyleRef failedStyle = ListItemStyle::create("failed");

    const char *checkBoxPart = "bubble.check";
    const char *bubbleContentPart = "bubble.content";
    const char *thumbContentPart = "info.thumbnail";
    const char *timeTextPart = "info.time";
    const char *draftButtonPart = "draft.button";
    const char *failedButtonPart = "failed.button";
    const char *infoStatus = "info.status";
    const char *msgType = "msg.type";
}

ConvListViewItem::ConvListViewItem(ConvItemType type)
    : ListItem()
    , m_SearchMode(false)
{
    updateItemType(type);
}

ConvListViewItem::~ConvListViewItem()
{

}

std::string ConvListViewItem::getText(ListItem &item, const char *part)
{
    if(!strcmp(part, timeTextPart))
        return getTime();
    else if(!strcmp(part, msgType))
        return getMsgType();
    else
        return "";
}

Evas_Object *ConvListViewItem::getContent(ListItem &item, const char *part)
{
    if(!strcmp(part, bubbleContentPart))
    {
        return getBubbleContent();
    }
    else if(!strcmp(part, thumbContentPart))
    {
        return getThumbnail();
    }
    else if(!strcmp(part, draftButtonPart))
    {
        return createButton(!getOwner()->getCheckMode(), Draft);
    }
    else if(!strcmp(part, failedButtonPart))
    {
        return createButton(!getOwner()->getCheckMode(), Failed);
    }
    else if(!strcmp(part, infoStatus))
    {
        return getProgress();
    }
    return nullptr;
}

const char *ConvListViewItem::getCheckPart(ListItem &item)
{
    return checkBoxPart;
}

Evas_Object *ConvListViewItem::createButton(bool isEnabled, ConvItemType type)
{
    Evas_Object *button = elm_button_add(*getOwner());
    evas_object_event_callback_add
    (
        button,
        EVAS_CALLBACK_MOUSE_DOWN,
        [](void *data, Evas *e, Evas_Object *obj, void *event_info)
        {
            if(!elm_object_disabled_get(obj))
                View::emitSignal(elm_object_content_get(obj), "pressed", "*");
        },
        this
    );
    evas_object_event_callback_add
    (
        button,
        EVAS_CALLBACK_MOUSE_UP,
        [](void *data, Evas *e, Evas_Object *obj, void *event_info)
        {
            if(!elm_object_disabled_get(obj))
                View::emitSignal(elm_object_content_get(obj), "unpressed", "*");
        },
        this
    );

    elm_object_style_set(button, "transparent");
    Evas_Object *icon =  nullptr;

    if(type == Draft)
    {
        icon = View::addLayout(button, ICONS_EDJ_PATH, "draft_icon");
        evas_object_smart_callback_add(button, "clicked", SMART_CALLBACK(ConvListViewItem, onEditButtonClicked), this);
    }
    else if(type == Failed)
    {
        icon = View::addLayout(button, ICONS_EDJ_PATH, "failed_icon");
        evas_object_smart_callback_add(button, "clicked", SMART_CALLBACK(ConvListViewItem, onFailedButtonClicked), this);
    }

    elm_object_content_set(button, icon);
    evas_object_propagate_events_set(button, !isEnabled);
    elm_object_disabled_set(button, !isEnabled);

    return button;
}

Evas_Object *ConvListViewItem::createProgress()
{
    Evas_Object *progressbar = elm_progressbar_add(*getOwner());
    elm_object_style_set(progressbar, "process_small");
    evas_object_show(progressbar);
    elm_progressbar_pulse(progressbar, EINA_TRUE);
    return progressbar;
}

void ConvListViewItem::showSearch()
{
    m_SearchMode = true;
    if(getElmObjItem())
        emitSignal("show_search", "*");
}

void ConvListViewItem::onRealized(ListItem &item)
{
    if(m_SearchMode)
        emitSignal("show_search", "*");
}

void ConvListViewItem::onUnrealized(ListItem &item)
{
    // Use because genlist does not save state for other items
    if(m_SearchMode)
        emitSignal("hide_search", "*");
}

void ConvListViewItem::updateProgressField()
{
    updateFields(infoStatus, ELM_GENLIST_ITEM_FIELD_CONTENT);
}

void ConvListViewItem::updateItemType(ConvItemType type)
{
    switch (type)
    {
        case Sent:
            setStyle(sentStyle);
            break;
        case Received:
            setStyle(receivedStyle);
            break;
        case Draft:
            setStyle(draftStyle);
            break;
        case Failed:
            setStyle(failedStyle);
            break;
        default:
            break;
    }
}
