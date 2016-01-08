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

#include "PopupManager.h"
#include "Window.h"
#include "CallbackAssist.h"

#include <efl_extension.h>

using namespace Msg;

PopupManager::PopupManager(Window &window)
    : m_pPopup(nullptr)
    , m_pCtxPopup(nullptr)
    , m_Window(window)
{

}

Window &PopupManager::getWindow() const
{
    return m_Window;
}

PopupManager::~PopupManager()
{
}

bool PopupManager::isVisible() const
{
    return isPopupVisible() || isCtxPopupVisible();
}

void PopupManager::reset()
{
    resetCtxPopup();
    resetPopup();
}

Popup &PopupManager::getPopup()
{
    if(!m_pPopup)
    {
        m_pPopup = new Popup(*this);
        eext_object_event_callback_add(*m_pPopup, EEXT_CALLBACK_BACK, SMART_CALLBACK(PopupManager, onHwBackButtonPopupClicked), this);
    }

    return *m_pPopup;
}

bool PopupManager::isPopupVisible() const
{
    return m_pPopup ? m_pPopup->isVisible() : false;
}

void PopupManager::resetPopup()
{
    if(m_pPopup)
    {
        m_pPopup->View::destroy();
        m_pPopup = nullptr;
    }
}

ContextPopup &PopupManager::getCtxPopup()
{
    if(!m_pCtxPopup)
    {
        m_pCtxPopup = new ContextPopup(*this);
        eext_object_event_callback_add(*m_pCtxPopup, EEXT_CALLBACK_BACK, SMART_CALLBACK(PopupManager, onHwBackButtonCtxPopupClicked), this);
    }

    return *m_pCtxPopup;
}

bool PopupManager::isCtxPopupVisible() const
{
    return m_pCtxPopup ? m_pCtxPopup->isVisible() : false;
}

void PopupManager::resetCtxPopup()
{
    if(m_pCtxPopup)
    {
        m_pCtxPopup->View::destroy();
        m_pCtxPopup = nullptr;
    }
}

void PopupManager::onHwBackButtonPopupClicked(Evas_Object *obj, void *eventInfo)
{
    resetPopup();
}

void PopupManager::onHwBackButtonCtxPopupClicked(Evas_Object *obj, void *eventInfo)
{
    resetCtxPopup();
}

