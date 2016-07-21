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

#include "BubbleViewItem.h"
#include "Logger.h"

using namespace Msg;

const int tapFingerSize = 12;

BubbleViewItem::BubbleViewItem(BubbleEntity &entity)
    : m_Entity(entity)
    , m_pListener(nullptr)
{

}

BubbleViewItem::~BubbleViewItem()
{

}

void BubbleViewItem::setListener(IBubbleViewItemListener *l)
{
    m_pListener = l;
}

BubbleEntity &BubbleViewItem::getEntity()
{
    return m_Entity;
}

void BubbleViewItem::attachGestureTapLayer(Evas_Object *parent, Evas_Object *obj)
{
    Evas_Object *layer = elm_gesture_layer_add(parent);
    evas_object_show(layer);
    elm_gesture_layer_attach(layer, obj);
    elm_gesture_layer_tap_finger_size_set(layer, ELM_SCALE_SIZE(tapFingerSize));

    elm_gesture_layer_cb_add
    (
        layer,
        ELM_GESTURE_N_TAPS,
        ELM_GESTURE_STATE_END,
        [](void *data, void *event_info)->Evas_Event_Flags
        {
            auto *self = static_cast<BubbleViewItem*>(data);
            if(self)
                self->emitActionEvent();
            return EVAS_EVENT_FLAG_NONE;
        },
        this
    );
}

void BubbleViewItem::emitActionEvent()
{
    if(m_pListener)
        m_pListener->onAction(*this);
}
