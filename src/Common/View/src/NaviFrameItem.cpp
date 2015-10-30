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


#include "NaviFrameItem.h"
#include "Logger.h"

using namespace Msg;

namespace
{
    const char *okButtonPart = "title_right_btn";
    const char *cancelButtonPart = "title_left_btn";
    const char *prevButtonPart = "elm.swallow.prev_btn";
    const char *okButtonStyle = "naviframe/title_done";
    const char *cancelButtonStyle = "naviframe/title_cancel";
    const char *prevButtonStyle = "naviframe/back_btn/default";
    const char *badgePart = "title_badge";
    const char *buttonTypeKey = "BTkey";
}

NaviFrameItem::NaviFrameItem(NaviFrameView &owner)
    : ViewItem()
    , m_Owner(owner)
{
    m_pNaviBar = new NaviBar(*this);
}

NaviFrameItem::~NaviFrameItem()
{
    delete m_pNaviBar;
}

const NaviFrameItem::NaviBar &NaviFrameItem::getNaviBar() const
{
    return *m_pNaviBar;
}

NaviFrameItem::NaviBar &NaviFrameItem::getNaviBar()
{
    return *m_pNaviBar;
}

NaviFrameView &NaviFrameItem::getOwner()
{
    return m_Owner;
}

const NaviFrameView &NaviFrameItem::getOwner() const
{
    return m_Owner;
}

NaviFrameItem::NaviBar::NaviBar(NaviFrameItem &onwer)
    : m_Owner(onwer)
    , ButtonList()
{
    ButtonList[NaviCancelButtonId] = ButtonStruct(nullptr, cancelButtonPart, cancelButtonStyle);
    ButtonList[NaviOkButtonId] = ButtonStruct(nullptr, okButtonPart, okButtonStyle);
    ButtonList[NaviPrevButtonId] = ButtonStruct(nullptr, prevButtonPart, prevButtonStyle);
}

NaviFrameItem::NaviBar::~NaviBar()
{
}

NaviFrameItem &NaviFrameItem::NaviBar::getOwner()
{
    return m_Owner;
}

const NaviFrameItem &NaviFrameItem::NaviBar::getOwner() const
{
    return m_Owner;
}

void NaviFrameItem::NaviBar::show(bool value, bool transition)
{
    elm_naviframe_item_title_enabled_set(m_Owner.getElmObjItem(), value, transition);
}

bool NaviFrameItem::NaviBar::isVisible()
{
    return elm_naviframe_item_title_enabled_get(m_Owner.getElmObjItem());
}

void NaviFrameItem::NaviBar::setTitle(const std::string &title)
{
    m_Owner.setText(title);
}

void NaviFrameItem::NaviBar::setTitle(const TText &title)
{
    m_Owner.setText(title);
}

std::string NaviFrameItem::NaviBar::getTitle() const
{
    return m_Owner.getText();
}

void NaviFrameItem::NaviBar::setBadge(const std::string &badge)
{
    elm_object_item_part_text_set(m_Owner.getElmObjItem(), badgePart, badge.c_str());
}

void NaviFrameItem::NaviBar::showBadge(bool value)
{
    const char *sig = value ? "elm,state,title_badge,show" :"elm,state,title_badge,hide";
    m_Owner.emitSignal(sig);
}

std::string NaviFrameItem::NaviBar::getBadge() const
{
    const char *text = elm_object_item_part_text_get(m_Owner.getElmObjItem(), badgePart);
    return text ? text : std::string();
}

void NaviFrameItem::NaviBar::getButton(NaviButtonId id)
{
    if(!ButtonList[id].button)
    {
        Evas_Object *parent = getOwner().getWidget();
        Evas_Object *btn = elm_button_add(parent);
        ButtonList[id].button = btn;
        elm_object_style_set(btn, ButtonList[id].style);
        evas_object_data_set(btn, buttonTypeKey, (void*)id);
        evas_object_smart_callback_add(btn, "clicked", on_button_clicked, this);
    }
}

void NaviFrameItem::NaviBar::showButton(NaviButtonId id, bool value)
{
    if(value)
    {
        getButton(id);

        if(!getOwner().getContent(ButtonList[id].part))
            getOwner().setContent(ButtonList[id].button, ButtonList[id].part);

        evas_object_show(ButtonList[id].button);
    }
    else
    {
        evas_object_hide(ButtonList[id].button);
    }

    switch(id)
    {
        case NaviCancelButtonId:
            showOkButtonPart(value);
            break;

        case NaviOkButtonId:
            showCancelButtonPart(value);
            break;

        default:
            break;
    }
}

void NaviFrameItem::NaviBar::disabledButton(NaviButtonId id, bool value)
{
    elm_object_disabled_set(ButtonList[id].button, value);
}

void NaviFrameItem::NaviBar::showCancelButtonPart(bool value)
{
    const char *sig = value ? "elm,state,title_left_btn,show" : "elm,state,title_left_btn,hide";
    m_Owner.emitSignal(sig);
}

void NaviFrameItem::NaviBar::showOkButtonPart(bool value)
{
    const char *sig = value ? "elm,state,title_right_btn,show" : "elm,state,title_right_btn,hide";
    m_Owner.emitSignal(sig);
}

void NaviFrameItem::NaviBar::on_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
    NaviFrameItem::NaviBar *naviBar = static_cast<NaviFrameItem::NaviBar*>(data);
    int type = (int)evas_object_data_get(obj, buttonTypeKey);
    naviBar->getOwner().onButtonClicked(naviBar->getOwner(), (NaviButtonId)type);
}
