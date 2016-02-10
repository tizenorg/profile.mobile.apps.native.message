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

#include "Viewer.h"
#include "Logger.h"
#include "PathUtils.h"
#include "Logger.h"
#include "MsgEngine.h"
#include "App.h"

#include <Elementary.h>
#include <sstream>

using namespace Msg;

Viewer::Viewer(NaviFrameController &parent, MsgId id)
    : FrameController(parent)
    , m_MsgId(id)
    , m_pLayout(nullptr)
    , m_pPlayerControl(nullptr)
{
    create();
}

Viewer::~Viewer()
{
    getApp().getContactManager().removeListener(*this);
}

void Viewer::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    updateNavibar();
    setContent(*m_pLayout);
}

void Viewer::updateNavibar()
{
    getNaviBar().clear();
    getNaviBar().setColor(NaviBar::NaviBlueColorId);
    getNaviBar().showButton(NaviPrevButtonId, true);
    MessageRef msg = getMsgEngine().getStorage().getMessage(m_MsgId);
    if(msg)
        FrameController::setNaviBarTitle(msg->getAddressList());
}

void Viewer::create()
{
    getApp().getContactManager().addListener(*this);
    createLayout();
    createPlayerControl();
    setHwButtonListener(*m_pLayout, this);
}

void Viewer::createLayout()
{
    m_pLayout = new ViewerLayout(getParent());
    m_pLayout->show();
}

void Viewer::createPlayerControl()
{
    m_pPlayerControl = new PlayerControl(*m_pLayout);
    m_pPlayerControl->setStartTime("00:00");
    m_pPlayerControl->setEndTime("00:00"); // For test
    m_pPlayerControl->setListener(this);
    m_pPlayerControl->show();
    m_pLayout->setPlayerControl(*m_pPlayerControl);
}

void Viewer::onHwBackButtonClicked()
{
    getParent().pop();
}

void Viewer::onHwMoreButtonClicked()
{

}

void Viewer::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    MSG_LOG("");
    switch(buttonId)
    {
        case NaviCenterButtonId:
            break;

        case NaviPrevButtonId:
            getParent().pop();
            break;

        case NaviDownButtonId:
            break;

        default:
            break;
    };
}

void Viewer::onPlayClicked()
{
    MSG_LOG("");
    m_pPlayerControl->setPlayState(PlayerControl::PauseState);
}

void Viewer::onPauseClicked()
{
    MSG_LOG("");
    m_pPlayerControl->setPlayState(PlayerControl::PlayState);
}

void Viewer::onNextClicked()
{
    MSG_LOG("");
}

void Viewer::onPrevClicked()
{
    MSG_LOG("");
}

void Viewer::onContactChanged()
{
    MSG_LOG("");
    updateNavibar();
}
