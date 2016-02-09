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

#include <Elementary.h>
#include <sstream>

using namespace Msg;

Viewer::Viewer(NaviFrameController &parent, MsgId id)
    : FrameController(parent)
    , m_MsgId(id)
    , m_pLayout(nullptr)
{
    create();
}

Viewer::~Viewer()
{
}

void Viewer::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    updateNavibar();
    setContent(*m_pLayout);
}

void Viewer::updateNavibar()
{
    getNaviBar().setTitle("Viewer");
    getNaviBar().setColor(NaviBar::NaviBlueColorId);
    getNaviBar().showButton(NaviPrevButtonId, true);
}

void Viewer::create()
{
    createLayout();
    setHwButtonListener(*m_pLayout, this);
}

void Viewer::createLayout()
{
    m_pLayout = new ViewerLayout(getParent());
    m_pLayout->show();
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
    if(buttonId == NaviPrevButtonId)
        getParent().pop();
}
