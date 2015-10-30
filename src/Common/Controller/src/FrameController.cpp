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


#include "FrameController.h"
#include "NaviFrameController.h"
#include "Logger.h"
#include "App.h"

using namespace Msg;

FrameController::FrameController(NaviFrameController &parent)
    : ViewItemController(&parent)
    , NaviFrameItem(parent.getView())
    , m_pPopup(nullptr)
    , m_pCtxPopup(nullptr)
{
    MSG_LOG("Constructor: ", this);
}

FrameController::~FrameController()
{
    MSG_LOG("Destructor: ", this);

    resetPopup();
    resetCtxPopup();
}

NaviFrameController &FrameController::getParent()
{
    return static_cast<NaviFrameController &>(ViewItemController::getParent());
}

NaviFrameItem &FrameController::getViewItem()
{
    return *this;
}

Popup &FrameController::getPopup()
{
    if(!m_pPopup)
    {
        m_pPopup = new Popup(getApp().getWindow().getWinEvasObject());
        m_pPopup->setListener(this);
    }

    return *m_pPopup;
}

void FrameController::resetPopup()
{
    if(m_pPopup)
    {
        m_pPopup->destroy();
        m_pPopup = nullptr;
    }
}

ContextPopup &FrameController::getCtxPopup()
{
    if(!m_pCtxPopup)
    {
        m_pCtxPopup = new ContextPopup(getApp().getWindow().getWinEvasObject());
        m_pCtxPopup->setListener(this);
    }

    return *m_pCtxPopup;
}

void FrameController::resetCtxPopup()
{
    if(m_pCtxPopup)
    {
        m_pCtxPopup->destroy();
        m_pCtxPopup = nullptr;
    }
}



