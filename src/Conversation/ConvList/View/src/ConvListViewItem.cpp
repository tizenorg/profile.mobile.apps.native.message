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

    const char *draftButtonStyle = "edit_button";
    const char *failedButtonStyle = "resend_button";
}

ConvListViewItem::ConvListViewItem(ConvItemType type)
    : ListItem()
    , m_BubbleWidth(0)
    , m_BubbleHeight(0)
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

ConvListViewItem::~ConvListViewItem()
{

}

std::string ConvListViewItem::getText(ListItem &item, const char *part)
{
    if(!strcmp(part, "elm.text"))
        return getText();
    else if(!strcmp(part, timeTextPart))
        return getTime();
    else
        return "";
}

Evas_Object *ConvListViewItem::getContent(ListItem &item, const char *part)
{
    if(!strcmp(part, bubbleContentPart))
    {
        Evas_Object *bubble = getBubbleContent();
        evas_object_event_callback_add(bubble, EVAS_CALLBACK_RESIZE, EVAS_EVENT_CALLBACK(ConvListViewItem, onBubbleResized), this);
        if(m_BubbleHeight > 0 && m_BubbleWidth > 0)
        {
            evas_object_size_hint_min_set(bubble, m_BubbleWidth, m_BubbleHeight);
            evas_object_size_hint_max_set(bubble, m_BubbleWidth, m_BubbleHeight);
        }
        return bubble;
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
    Evas_Object *button = nullptr;
    if(type == Draft || type == Failed)
    {
        button = elm_button_add(*getOwner());
        if(type == Draft)
        {
            elm_object_style_set(button, draftButtonStyle);
            evas_object_smart_callback_add(button, "clicked", SMART_CALLBACK(ConvListViewItem, onEditButtonClicked), this);
        }
        else if(type == Failed)
        {
            elm_object_style_set(button, failedButtonStyle);
            evas_object_smart_callback_add(button, "clicked", SMART_CALLBACK(ConvListViewItem, onFailedButtonClicked), this);
        }
        View::expand(button);
        evas_object_show(button);

        evas_object_propagate_events_set(button, !isEnabled);
        elm_object_disabled_set(button, !isEnabled);

    }
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

void ConvListViewItem::updateProgressField()
{
    updateFields(infoStatus, ELM_GENLIST_ITEM_FIELD_CONTENT);
}

void ConvListViewItem::onBubbleResized(Evas_Object *obj, void *data)
{
    MSG_LOG("");
    Evas_Coord w,h;
    evas_object_geometry_get(obj, nullptr, nullptr, &w, &h);
    if(m_BubbleHeight < h || m_BubbleWidth < w)
    {
        m_BubbleWidth = w;
        m_BubbleHeight = h;
        elm_genlist_item_update(this->getElmObjItem());
    }
}
