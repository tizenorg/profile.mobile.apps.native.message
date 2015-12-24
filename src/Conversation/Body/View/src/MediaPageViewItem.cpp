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

#include "MediaPageViewItem.h"
#include "PageView.h"

#include <assert.h>

using namespace Msg;

namespace
{
    const char *mediaMainGroup = "conv/body/media_main";
}

MediaPageViewItem::MediaPageViewItem(PageView &parent, const std::string &resourcePath)
    : PageViewItem(parent)
    , m_pListener(nullptr)
    , m_pButton(nullptr)
    , m_ResourcePath(resourcePath)
{
    View::setEo(createMainLayout(parent.getItemParent()));
    Evas_Object *button = createButton(getEo());
    setContent(button, "swl.content");
}

const std::string &MediaPageViewItem::getResourcePath() const
{
    return m_ResourcePath;
}

MediaPageViewItem::~MediaPageViewItem()
{

}

template<class...Args>
void MediaPageViewItem::notifyListener(void *data, void (IMediaPageViewItemListener::*method)(MediaPageViewItem &, Args...args), Args&&...args)
{
    MediaPageViewItem *self = static_cast<MediaPageViewItem*>(data);
    if(self && self->m_pListener)
        (self->m_pListener->*method)(*self, args...);
}

Evas_Object *MediaPageViewItem::getMediaLayout() const
{
    return getEo();
}

Evas_Object *MediaPageViewItem::getButtonLayout() const
{
    return m_pButton;
}

void MediaPageViewItem::setButtonContent(Evas_Object *layout)
{
    elm_object_part_content_set(m_pButton, "elm.swallow.content", layout);
}

void MediaPageViewItem::setRect(Evas_Object *layout)
{
    setContent(layout, "swl.rect");
}

void MediaPageViewItem::setListener(IMediaPageViewItemListener *l)
{
    m_pListener = l;
}

Evas_Object *MediaPageViewItem::createMainLayout(Evas_Object *parent)
{
    Evas_Object *layout = elm_layout_add(parent);
    elm_layout_file_set(layout, getEdjPath().c_str(), mediaMainGroup);
    evas_object_show(layout);
    return layout;
}

Evas_Object *MediaPageViewItem::createGestureLayer(Evas_Object *parent)
{
    // TODO: impl if needed
    return nullptr;
}

Evas_Object *MediaPageViewItem::createButton(Evas_Object *parent)
{
    Evas_Object *button = elm_button_add(parent);
    m_pButton = button;
    elm_object_style_set(button, "focus");
    evas_object_show(button);

    evas_object_smart_callback_add(button, "clicked", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onClicked);
    }, this);

    evas_object_smart_callback_add(button, "pressed", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onPressed);
    }, this);

    evas_object_smart_callback_add(button, "focused", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onFocused);
    }, this);

    evas_object_smart_callback_add(button, "unfocused", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onUnfocused);
    }, this);

    evas_object_smart_callback_add(button, "unpressed", [](void *data, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onUnpressed);
    }, this);

    evas_object_event_callback_add(button, EVAS_CALLBACK_KEY_DOWN, [](void *data, Evas *e, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onKeyDown, *(Evas_Event_Key_Down*)event_info);
    }, this);

    evas_object_event_callback_add(button, EVAS_CALLBACK_KEY_UP, [](void *data, Evas *e, Evas_Object *obj, void *event_info)
    {
        notifyListener(data, &IMediaPageViewItemListener::onKeyUp, *(Evas_Event_Key_Up*)event_info);
    }, this);

    return button;
}

void MediaPageViewItem::onBeforeDelete(View &view)
{
    if(m_pListener)
        m_pListener->onDelete(*this);
}
