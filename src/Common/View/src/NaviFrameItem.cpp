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
#include "NaviFrameView.h"
#include "Logger.h"
#include "TextDecorator.h"

using namespace Msg;

namespace
{
    const char *titleStyleName = "title";
    const char *naviTitlePart = "title";
    const char *titleTextPart = "text.center";
    const char *okButtonPart = "swallow.right";
    const char *cancelButtonPart = "swallow.left";
    const char *prevButtonPart = "swallow.left";
    const char *downButtonPart = "swallow.right";
    const char *centerButtonPart = "swallow.center";
    const char *okButtonStyle = "naviframe/title_right";
    const char *cancelButtonStyle = "naviframe/title_left";
    const char *centerButtonStyle = "naviframe/title_center";
    const char *prevButtonStyle = "naviframe/arrow_back";
    const char *downButtonStyle = "naviframe/arrow_down";
    const char *cancelButtonDefTextId = "IDS_MSG_ACBUTTON_CANCEL_ABB";
    const char *okButtonDefTextId = "IDS_MSG_ACBUTTON_DONE_ABB";
    const char *buttonTypeKey = "BTkey";
    const char *textColorWhiteTitleButtons = "#2e8b99";
    const char *textColorBlueTitleButtons = "#fafafa";
    const int textSizeCenterButton = 50;
    const int textSizeLateralButton = 35;
}

NaviFrameItem::NaviFrameItem(NaviFrameView &owner)
    : ViewItem()
    , m_Owner(owner)
{
    m_pNaviBar = new NaviBar(*this);
}

NaviFrameItem::~NaviFrameItem()
{
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

void NaviFrameItem::setTitleVisibility(bool visible)
{
    if (visible)
    {
        setContent(*m_pNaviBar, naviTitlePart);
        evas_object_show(*m_pNaviBar);
    }
    else
    {
        evas_object_hide(elm_object_item_part_content_unset(getElmObjItem(), naviTitlePart));
    }
}

NaviFrameItem::NaviBar::NaviBar(NaviFrameItem &onwer)
    : m_Owner(onwer)
    , m_ButtonList()
    , m_CurrentColor(NaviBlueColorId)
    , m_SearchBar(nullptr)
{
    setEo(addLayout(m_Owner.getOwner(), MSG_TITLE_EDJ_PATH, titleStyleName));
    m_ButtonList[NaviCancelButtonId] = ButtonStruct(nullptr, cancelButtonPart, cancelButtonStyle, cancelButtonDefTextId);
    m_ButtonList[NaviOkButtonId] = ButtonStruct(nullptr, okButtonPart, okButtonStyle, okButtonDefTextId);
    m_ButtonList[NaviCenterButtonId] = ButtonStruct(nullptr, centerButtonPart, centerButtonStyle);
    m_ButtonList[NaviPrevButtonId] = ButtonStruct(nullptr, prevButtonPart, prevButtonStyle);
    m_ButtonList[NaviExpandButtonId] = ButtonStruct(nullptr, downButtonPart, downButtonStyle);
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

void NaviFrameItem::NaviBar::setVisible(bool visible)
{
    getOwner().setTitleVisibility(visible);
}

void NaviFrameItem::NaviBar::setTitle(const std::string &title)
{
    setText(title, titleTextPart);
}

void NaviFrameItem::NaviBar::setTitle(const TText &title)
{
    setText(title, titleTextPart);
}

std::string NaviFrameItem::NaviBar::getTitle() const
{
    return getText(titleTextPart);
}

void NaviFrameItem::NaviBar::getButton(NaviButtonId id)
{
    if(!m_ButtonList[id].button)
    {
        Evas_Object *btn = elm_button_add(getEo());
        m_ButtonList[id].button = btn;
        elm_object_style_set(btn, m_ButtonList[id].style);
        setButtonText(id, msgt(m_ButtonList[id].default_text_id));
        evas_object_data_set(btn, buttonTypeKey, (void*)id);
        evas_object_smart_callback_add(btn, "clicked", on_button_clicked, this);
    }
}

void NaviFrameItem::NaviBar::showButton(NaviButtonId id, bool value)
{
    if(value)
    {
        getButton(id);

        if(getContent(m_ButtonList[id].part) != m_ButtonList[id].button)
            setContent(m_ButtonList[id].button, m_ButtonList[id].part);
        setButtonColor(id, m_CurrentColor);
        evas_object_show(m_ButtonList[id].button);
    }
    else
    {
        if(getContent(m_ButtonList[id].part) == m_ButtonList[id].button)
        {
            evas_object_del(m_ButtonList[id].button);
            m_ButtonList[id].button = nullptr;
        }
    }

    switch(id)
    {
        case NaviCancelButtonId:
            showCancelButtonPart(value);
            break;

        case NaviOkButtonId:
            showOkButtonPart(value);
            break;

        case NaviCenterButtonId:
            showCenterButtonPart(value);
            break;

        case NaviPrevButtonId:
            showPrevButtonPart(value);
            break;

        case NaviExpandButtonId:
            showDownButtonPart(value);
            break;

        default:
            break;
    }
}

void NaviFrameItem::NaviBar::disabledButton(NaviButtonId id, bool value)
{
    elm_object_disabled_set(m_ButtonList[id].button, value);
}

void NaviFrameItem::NaviBar::showCancelButtonPart(bool value)
{
    if(value)
    {
        emitSignal("cancel,show,btn", "*");
    }
    else if(getContent(cancelButtonPart) == nullptr)
    {
        emitSignal("left,clear", "*");
    }
}

void NaviFrameItem::NaviBar::showOkButtonPart(bool value)
{
    if(value)
        emitSignal("done,show,btn", "*");
    else if(getContent(okButtonPart) == nullptr)
        emitSignal("right,clear", "*");
}

void NaviFrameItem::NaviBar::showCenterButtonPart(bool value)
{
    const char *sig = value ? "center,show,btn" : "center,hide,btn";
    emitSignal(sig, "*");
}

void NaviFrameItem::NaviBar::showPrevButtonPart(bool value)
{
    if(value)
        emitSignal("back,show,btn", "*");
    else if(getContent(prevButtonPart) == nullptr)
        emitSignal("left,clear", "*");

    if(getContent(downButtonPart) == nullptr)
        emitSignal("empty,right,show", "*");
}

void NaviFrameItem::NaviBar::showDownButtonPart(bool value)
{
    if(value)
        emitSignal("down,show,btn", "*");
    else if(getContent(downButtonPart) == nullptr)
        emitSignal("right,clear", "*");
}

int NaviFrameItem::NaviBar::getTextSize(NaviFrameItem::NaviButtonId id) const
{
    return id == NaviFrameItem::NaviCenterButtonId ? textSizeCenterButton : textSizeLateralButton;
}

void NaviFrameItem::NaviBar::on_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
    NaviFrameItem::NaviBar *naviBar = static_cast<NaviFrameItem::NaviBar*>(data);
    int type = (int)(intptr_t)evas_object_data_get(obj, buttonTypeKey);
    naviBar->getOwner().onButtonClicked(naviBar->getOwner(), (NaviButtonId)type);
}

void NaviFrameItem::onAttached(ViewItem &item)
{
    ViewItem::onAttached(item);
    setContent(*m_pNaviBar, naviTitlePart);
}

void NaviFrameItem::NaviBar::setColor(NaviColorId id)
{
    switch(id)
    {
        case NaviBlueColorId:
            emitSignal("title,color,blue", "*");
            m_CurrentColor = NaviBlueColorId;
            break;

        case NaviWhiteColorId:
            emitSignal("title,color,white", "*");
            m_CurrentColor = NaviWhiteColorId;
            break;

        default:
            break;
    }
}

void NaviFrameItem::NaviBar::setButtonText(NaviButtonId id, const std::string &text)
{
    //TODO: implement text color in edc
    TextStyle style;

    switch(m_CurrentColor)
    {
        case NaviBlueColorId:
            style.setColor(textColorBlueTitleButtons);
            break;

        case NaviWhiteColorId:
            style.setColor(textColorWhiteTitleButtons);
            break;

        default:
            break;
    }
    style.setSize(getTextSize(id));
    elm_object_text_set(m_ButtonList[id].button, TextDecorator::make(text, style).c_str());
}

void NaviFrameItem::NaviBar::setButtonText(NaviButtonId id, const TText &text)
{
    setText(m_ButtonList[id].button, text);
    //TODO: implement text color in edc
    setButtonColor(id, m_CurrentColor);
}

void NaviFrameItem::NaviBar::setButtonColor(NaviButtonId id, NaviColorId titleColor)
{
    // Note that, button's color is opposite to title's color
    if (id == NaviPrevButtonId || id == NaviExpandButtonId)
    {
        switch(titleColor)
        {
            case NaviBlueColorId:
                emitSignal(m_ButtonList[id].button, "button,color,white", "*");
                break;

            case NaviWhiteColorId:
                emitSignal(m_ButtonList[id].button, "button,color,blue", "*");
                break;

            default:
                break;
        }
    }
    else
    {
        TextStyle style;

        switch(titleColor)
        {
            case NaviBlueColorId:
                style.setColor(textColorBlueTitleButtons);
                break;

            case NaviWhiteColorId:
                style.setColor(textColorWhiteTitleButtons);
                break;

            default:
                break;
        }
        style.setSize(getTextSize(id));
        const char *buttonText = elm_object_text_get(m_ButtonList[id].button);
        if (buttonText != nullptr)
        {
            elm_object_text_set(m_ButtonList[id].button, TextDecorator::make(buttonText, style).c_str());
        }
    }
}

void NaviFrameItem::NaviBar::clear()
{
    emitSignal("sides,clear", "*");
    for (int iter = NaviCancelButtonId; iter < NaviButtonMax; iter++)
    {
        evas_object_hide(elm_object_part_content_unset(getEo(), m_ButtonList[iter].part));
    }
    showCenterButtonPart(false);
}

void NaviFrameItem::NaviBar::setSearch(Evas_Object *searchPanel)
{
    m_SearchBar = searchPanel;
}

Evas_Object *NaviFrameItem::NaviBar::getSearch()
{
    return m_SearchBar;
}

const Evas_Object *NaviFrameItem::NaviBar::getSearch() const
{
    return m_SearchBar;
}

void NaviFrameItem::NaviBar::showSearch()
{
    emitSignal("search,show", "*");
    setContent(m_SearchBar, m_ButtonList[NaviCenterButtonId].part);
}

void NaviFrameItem::NaviBar::hideSearch()
{
    showButton(NaviCenterButtonId, false);
    showDownButtonPart(false);
}

void NaviFrameItem::NaviBar::setDownButtonState(bool expand)
{
    const char *sig = expand ? "button,expand" : "button,collapse";
    emitSignal(m_ButtonList[NaviExpandButtonId].button, sig, "*");
}
