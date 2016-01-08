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

#include "ContextPopup.h"
#include "Window.h"
#include "Logger.h"
#include "PopupManager.h"

using namespace Msg;

ContextPopupItem::ContextPopupItem(ContextPopup &parent, int id, ContextPopupItemPressedCb userCb, void *userData)
    : ViewItem()
    , m_ParentContextPopup(parent)
    , m_Id(id)
    , m_pUserCb(userCb)
    , m_pUserData(userData)
{

}

ContextPopupItem::~ContextPopupItem()
{
    MSG_LOG("Destructor");
}

ContextPopup &ContextPopupItem::getParent()
{
    return m_ParentContextPopup;
}

int ContextPopupItem::getId() const
{
    return m_Id;
}

// ContextPopup:
ContextPopup::ContextPopup(Evas_Object *parent)
    : m_pManager(nullptr)
{
    createContextPopup(parent);
}

ContextPopup::ContextPopup(PopupManager &parent)
    : m_pManager(&parent)
{
    createContextPopup(parent.getWindow());
}

ContextPopup::~ContextPopup()
{
    MSG_LOG("Destructor");
}

ContextPopupItem *ContextPopup::appendItem(const std::string &text, Evas_Object *icon, ContextPopupItemPressedCb cb, void *userData, int id)
{
    ContextPopupItem *newItem = new ContextPopupItem(*this, id, cb, userData);
    Elm_Object_Item *elmItem = elm_ctxpopup_item_append(getEo(), text.c_str(), icon, on_item_pressed_cb, newItem);

    if(elmItem)
    {
        newItem->setElmObjItem(elmItem);
    }
    else
    {
        delete newItem;
        newItem = nullptr;
    }

    return newItem;
}

void ContextPopup::createContextPopup(Evas_Object *parent)
{
    setEo(elm_ctxpopup_add(parent));

    //TODO: Should be uncommented when "more/default" style will be supported.
    //elm_object_style_set(getEo(), "more/default");
    elm_ctxpopup_horizontal_set(getEo(), EINA_FALSE);
    elm_ctxpopup_auto_hide_disabled_set(getEo(), EINA_TRUE);
    evas_object_smart_callback_add(getEo(), "dismissed", on_dismissed_cb, this);

    setDirectionPriority(ELM_CTXPOPUP_DIRECTION_UP,
                         ELM_CTXPOPUP_DIRECTION_UNKNOWN,
                         ELM_CTXPOPUP_DIRECTION_UNKNOWN,
                         ELM_CTXPOPUP_DIRECTION_UNKNOWN);
}

void ContextPopup::destroy()
{
    if(m_pManager)
        m_pManager->resetCtxPopup();
    else
        View::destroy();
}

void ContextPopup::setDirectionPriority(Elm_Ctxpopup_Direction first, Elm_Ctxpopup_Direction second,
                                        Elm_Ctxpopup_Direction third, Elm_Ctxpopup_Direction fourth)
{
    elm_ctxpopup_direction_priority_set(getEo(), first, second, third, fourth);
}

Elm_Ctxpopup_Direction ContextPopup::getDirection() const
{
    return elm_ctxpopup_direction_get(getEo());
}

void ContextPopup::on_dismissed_cb(void *data, Evas_Object *obj, void *event_info)
{
    ContextPopup *self = static_cast<ContextPopup*>(data);
    if(self->m_pManager)
        self->m_pManager->resetCtxPopup();
}

void ContextPopup::on_item_pressed_cb(void *data, Evas_Object *obj, void *event_info)
{
    ContextPopupItem *item = static_cast<ContextPopupItem*>(data);
    if(item && item->m_pUserCb)
        item->m_pUserCb(*item, item->m_pUserData);
}

void ContextPopup::align(Window &win)
{
    int w;
    int h;

    int pos = win.getRotation();
    win.getScreenSize(nullptr, nullptr, &w, &h);

    switch (pos)
    {
        case 0:
        case 180:
            move(w/2, h);
          break;
        case 90:
            move(h/2, w);
          break;
        case 270:
          move(h/2, w);
          break;
    }
}

