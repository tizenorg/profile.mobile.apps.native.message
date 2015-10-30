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

ContextPopup &ContextPopupItem::getParentContextPopup()
{
    return m_ParentContextPopup;
}

int ContextPopupItem::getId() const
{
    return m_Id;
}

// ContextPopup:
ContextPopup::ContextPopup(Evas_Object *parent)
    : View()
    , m_pListener(nullptr)
    , m_Type(0)
{
    createContextPopup(parent);
}

ContextPopup::~ContextPopup()
{
    MSG_LOG("Destructor");
}

void ContextPopup::setUserType(int type)
{
    m_Type = type;
}

int ContextPopup::getUserType() const
{
    return m_Type;
}

ContextPopupItem *ContextPopup::appendItem(int id, const std::string &text, Evas_Object *icon, ContextPopupItemPressedCb cb, void *userData)
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

void ContextPopup::setListener(IContextPopupListener *listener)
{
    m_pListener = listener;
}

void ContextPopup::createContextPopup(Evas_Object *parent)
{
    setEo(elm_ctxpopup_add(parent));
    elm_object_style_set(getEo(), "more/default");
    elm_ctxpopup_horizontal_set(getEo(), EINA_FALSE);
    elm_ctxpopup_auto_hide_disabled_set(getEo(), EINA_TRUE);
    evas_object_smart_callback_add(getEo(), "dismissed", on_dismissed_cb, this);

    setDirectionPriority(ELM_CTXPOPUP_DIRECTION_UP,
                         ELM_CTXPOPUP_DIRECTION_UNKNOWN,
                         ELM_CTXPOPUP_DIRECTION_UNKNOWN,
                         ELM_CTXPOPUP_DIRECTION_UNKNOWN);

    setHwButtonListener(getEo(), this);
}

void ContextPopup::dismiss()
{
    elm_ctxpopup_dismiss(getEo());
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

void ContextPopup::onHwBackButtonClicked()
{
    dismiss();
}

void ContextPopup::onHwMoreButtonClicked()
{
    dismiss();
}

void ContextPopup::on_dismissed_cb(void *data, Evas_Object *obj, void *event_info)
{
    ContextPopup * self = static_cast<ContextPopup*>(data);

    if(self && self->m_pListener)
        self->m_pListener->onContextPopupDismissed(*self);
}

void ContextPopup::on_item_pressed_cb(void *data, Evas_Object *obj, void *event_info)
{
    ContextPopupItem *item = static_cast<ContextPopupItem*>(data);

    if(item)
    {
        if(item->m_pUserCb)
        {
            item->m_pUserCb(item->getParentContextPopup(), *item, item->m_pUserData);
        }
        else if(item->getParentContextPopup().m_pListener)
        {
            item->getParentContextPopup().m_pListener->onContextPopupItemPressed(item->getParentContextPopup(), *item);
        }
    }
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


