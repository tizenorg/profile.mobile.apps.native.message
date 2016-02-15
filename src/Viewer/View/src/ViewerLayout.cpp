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

#include "ViewerLayout.h"

using namespace Msg;

namespace
{
    const char *layoutGroup = "viewer_layout";
    const char *bodyPart = "body";
    const char *playerPart = "swallow.playctrl";
    const char *subjectPart = "swallow.subject";
    const char *bgPart = "body.bg";
    const char *recipientsPart = "swallow.recipients";
    const char *touchAreaPart = "swallow.touch_area";
    const char *showRecipSig = "show_recipients";
    const char *hideRecipSig = "hide_recipients";
    const char *showPlayerSig = "show_player";
    const char *hidePlayerSig = "hide_player";
}

ViewerLayout::ViewerLayout(Evas_Object *parent)
    : m_pListener(nullptr)
{
    setEo(addLayout(parent, VIEWER_LAYOUT_EDJ_PATH, layoutGroup));

    Evas_Object *button = elm_button_add(getEo());
    elm_object_style_set(button, "transparent");
    evas_object_show(button);
    evas_object_smart_callback_add
    (
        button,
        "clicked",
        [](void *data, Evas_Object *obj, void *event_info)
        {
            ViewerLayout *self = static_cast<ViewerLayout*>(data);
            if(self->m_pListener)
                self->m_pListener->onLayoutTocuh();
        },
        this
    );

    setContent(button, touchAreaPart);
}

ViewerLayout::~ViewerLayout()
{

}

void ViewerLayout::setListener(IViewerLayoutListener *l)
{
    m_pListener = l;
}

void ViewerLayout::setPlayerControl(Evas_Object *obj)
{
    setContent(obj, playerPart);
}

void ViewerLayout::setSubject(Evas_Object *obj)
{
    setContent(obj, subjectPart);
}

void ViewerLayout::setBody(Evas_Object *obj)
{
    setContent(obj, bodyPart);
}

void ViewerLayout::setBg(Evas_Object *obj)
{
    setContent(obj, bgPart);
}

void ViewerLayout::setRecipients(Evas_Object *obj)
{
    setContent(obj, recipientsPart);
}

void ViewerLayout::showRecipients(bool show)
{
    const char *sig = show ? showRecipSig : hideRecipSig;
    emitSignal(sig, "");
}

void ViewerLayout::showPlayerControl(bool show)
{
    const char *sig = show ? showPlayerSig : hidePlayerSig;
    emitSignal(sig, "");
}

bool ViewerLayout::isPlayerControlVisible() const
{
    Evas_Object *obj = getContent(playerPart);
    return evas_object_visible_get(obj);
}

bool ViewerLayout::isRecipientsVisible() const
{
    Evas_Object *obj = getContent(recipientsPart);
    return evas_object_visible_get(obj);
}
