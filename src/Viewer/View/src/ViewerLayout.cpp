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
    const char *showRecipSig = "show_recipients";
    const char *hideRecipSig = "hide_recipients";
}

ViewerLayout::ViewerLayout(Evas_Object *parent)
{
    setEo(addLayout(parent, VIEWER_LAYOUT_EDJ_PATH, layoutGroup));
}

ViewerLayout::~ViewerLayout()
{

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
