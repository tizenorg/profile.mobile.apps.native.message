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
    const char *prevButtonStyle = "naviframe/arrow_back";
    const char *downButtonStyle = "naviframe/arrow_down";
    const char *cancelButtonDefTextId = "IDS_MSG_ACBUTTON_CANCEL_ABB";
    const char *okButtonDefTextId = "IDS_MSG_ACBUTTON_DONE_ABB";
    const char *badgePart = "title_badge";
    const char *buttonTypeKey = "BTkey";
    const char *textColorWhiteTitleButtons = "#3db9cc";
    const char *textColorBlueTitleButtons = "#fafafa";
    const int textSizeTitleButtons = 32;
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

NaviFrameItem::NaviBar::NaviBar(NaviFrameItem &onwer)
    : m_Owner(onwer)
    , ButtonList()
    , m_CurrentColor(NaviBlueColorId)
{
    ButtonList[NaviCancelButtonId] = ButtonStruct(nullptr, cancelButtonPart, cancelButtonStyle, cancelButtonDefTextId);
    ButtonList[NaviOkButtonId] = ButtonStruct(nullptr, okButtonPart, okButtonStyle, okButtonDefTextId);
    //TODO: implement style for center button
    ButtonList[NaviCenterButtonId] = ButtonStruct(nullptr, centerButtonPart, prevButtonStyle);
    ButtonList[NaviPrevButtonId] = ButtonStruct(nullptr, prevButtonPart, prevButtonStyle);
    ButtonList[NaviDownButtonId] = ButtonStruct(nullptr, downButtonPart, downButtonStyle);
}

NaviFrameItem::NaviBar::~NaviBar()
{
    for (int iter = NaviCancelButtonId; iter < NaviButtonMax; iter++)
    {
        if (ButtonList[iter].button != nullptr)
        {
            evas_object_del(ButtonList[iter].button);
        }
    }
}

NaviFrameItem &NaviFrameItem::NaviBar::getOwner()
{
    return m_Owner;
}

const NaviFrameItem &NaviFrameItem::NaviBar::getOwner() const
{
    return m_Owner;
}

//TODO: fix show (update title part in naviframe item manually)
void NaviFrameItem::NaviBar::setVisible(bool visible)
{
    const char *sig = visible ? "title,state,show" :"title,state,hide";
    emitSignal(sig, "*");
    elm_naviframe_item_title_visible_set(getOwner(), visible);
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
    if(!ButtonList[id].button)
    {
        Evas_Object *parent = getEo();
        Evas_Object *btn = elm_button_add(parent);
        ButtonList[id].button = btn;
        elm_object_style_set(btn, ButtonList[id].style);
        setButtonText(id, msgt(ButtonList[id].default_text_id));
        evas_object_data_set(btn, buttonTypeKey, (void*)id);
        evas_object_smart_callback_add(btn, "clicked", on_button_clicked, this);
    }
}

void NaviFrameItem::NaviBar::showButton(NaviButtonId id, bool value)
{
    if(value)
    {
        getButton(id);

        if(!getContent(ButtonList[id].part) != ButtonList[id].button)
            setContent(ButtonList[id].button, ButtonList[id].part);
        setButtonColor(id, m_CurrentColor);
        evas_object_show(ButtonList[id].button);
    }
    else
    {
        if (getContent(ButtonList[id].part) == ButtonList[id].button)
        {
            elm_object_part_content_unset(getEo(),ButtonList[id].part);
            evas_object_hide(ButtonList[id].button);
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

        case NaviDownButtonId:
            showDownButtonPart(value);
            break;

        default:
            break;
    }
}

void NaviFrameItem::NaviBar::disabledButton(NaviButtonId id, bool value)
{
    if (ButtonList[id].button == nullptr)
    {
        return;
    }
    elm_object_disabled_set(ButtonList[id].button, value);
}

void NaviFrameItem::NaviBar::showCancelButtonPart(bool value)
{
    const char *sig = value ? "cancel,show,btn" : "left,clear";
    emitSignal(sig, "*");
}

void NaviFrameItem::NaviBar::showOkButtonPart(bool value)
{
    const char *sig = value ? "done,show,btn" : "right,clear";
    emitSignal(sig, "*");
}

void NaviFrameItem::NaviBar::showCenterButtonPart(bool value, bool expand)
{
    const char *sig = value ? "center,show,btn" : "center,hide,btn";
    emitSignal(sig, "*");
}

void NaviFrameItem::NaviBar::showPrevButtonPart(bool value)
{
    const char *sig1 = value ? "back,show,btn" : "left,clear";
    const char *sig2 = value ? "empty,right,show" : "right,clear";
    emitSignal(sig1, "*");
    emitSignal(sig2, "*");
}

void NaviFrameItem::NaviBar::showDownButtonPart(bool value)
{
    const char *sig = value ? "down,show,btn" : "right,clear";
    emitSignal(sig, "*");
}

void NaviFrameItem::NaviBar::on_button_clicked(void *data, Evas_Object *obj, void *event_info)
{
    NaviFrameItem::NaviBar *naviBar = static_cast<NaviFrameItem::NaviBar*>(data);
    int type = (int)evas_object_data_get(obj, buttonTypeKey);
    naviBar->getOwner().onButtonClicked(naviBar->getOwner(), (NaviButtonId)type);
}

void NaviFrameItem::onViewItemCreated()
{
    ViewItem::onViewItemCreated();
    m_pNaviBar->initNaviBar();
    setContent(*m_pNaviBar, naviTitlePart);
}

void NaviFrameItem::NaviBar::initNaviBar()
{
    setEo(elm_layout_add(m_Owner.getOwner()));
    std::string edjPath = ResourceUtils::getResourcePath(MSG_TITLE_EDJ_PATH);
    elm_layout_file_set(getEo(), edjPath.c_str(), titleStyleName);
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
    if (ButtonList[id].button == nullptr)
    {
        return;
    }
    elm_object_text_set(ButtonList[id].button, text.c_str());
}

void NaviFrameItem::NaviBar::setButtonText(NaviButtonId id, const TText &text)
{
    if (ButtonList[id].button == nullptr)
    {
        return;
    }
    elm_object_domain_translatable_text_set(ButtonList[id].button, text.getDomain(), text.getMsg());
}

void NaviFrameItem::NaviBar::setButtonColor(NaviButtonId id, NaviColorId titleColor)
{
    if (ButtonList[id].button == nullptr)
    {
        return;
    }
    // Note that, button's color is opposite to title's color
    if (id == NaviPrevButtonId || id == NaviDownButtonId)
    {
        switch(titleColor)
        {
            case NaviBlueColorId:
                elm_object_signal_emit(ButtonList[id].button, "button,color,white", "*");
                break;

            case NaviWhiteColorId:
                elm_object_signal_emit(ButtonList[id].button, "button,color,blue", "*");
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
        style.setSize(textSizeTitleButtons);
        const char *buttonText = elm_object_text_get(ButtonList[id].button);
        if (buttonText != nullptr)
        {
            setButtonText(id, TextDecorator::make(buttonText, style).c_str());
        }
    }
}

void NaviFrameItem::NaviBar::clearBar()
{
    emitSignal("sides,clear", "*");
    for (int iter = NaviCancelButtonId; iter < NaviButtonMax; iter++)
    {
        evas_object_hide(elm_object_part_content_unset(getEo(), ButtonList[iter].part));
    }
}

void NaviFrameItem::NaviBar::switchToSearch(Evas_Object *searchPanel)
{
    //TODO: implement switchToSearch
}

void NaviFrameItem::NaviBar::expandDownButton(bool value)
{
    if (ButtonList[NaviDownButtonId].button == nullptr)
    {
        return;
    }
    if (!value)
    {
        elm_object_signal_emit(ButtonList[NaviDownButtonId].button, "button,expand", "*");
    }
    else
    {
        elm_object_signal_emit(ButtonList[NaviDownButtonId].button, "button,collapse", "*");
    }
}
