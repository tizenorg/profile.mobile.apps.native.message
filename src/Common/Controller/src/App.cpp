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

#include "App.h"
#include "ViewController.h"
#include "Logger.h"
#include "ContactManager.h"
#include "PathUtils.h"

using namespace Msg;

App::App()
    : m_pContactManager(nullptr)
    , m_pPopupManager(nullptr)
    , m_pThumbnailMaker(nullptr)
    , m_pSysSettingsManager(nullptr)
{
    int serviceResult = m_Engine.openService();
    if(serviceResult != MESSAGES_ERROR_NONE)
    {
        MSG_ENGINE_WHAT_ERROR(serviceResult);
        MSG_LOG_ERROR("Service handle open error = ", serviceResult);
    }
}

App::~App()
{
    delete m_pThumbnailMaker;
    delete m_pContactManager;
    delete m_pPopupManager;
    delete m_pSysSettingsManager;
}

bool App::init()
{
    getSysSettingsManager();
    getContactManager();
    getMsgEngine();
    getThumbnailMaker();

    std::string imagesPath = PathUtils::getResourcePath(IMAGES_EDJ_PATH);
    elm_theme_extension_add(nullptr, imagesPath.c_str());

    std::string bubbleThemePath = PathUtils::getResourcePath(BUBBLE_THEME_EDJ_PATH);
    elm_theme_extension_add(nullptr, bubbleThemePath.c_str());

    std::string buttonThemePath = PathUtils::getResourcePath(BUTTON_THEME_EDJ_PATH);
    elm_theme_extension_add(nullptr, buttonThemePath.c_str());

    return true;
}

MsgEngine &App::getMsgEngine()
{
    return m_Engine;
}

const MsgEngine &App::getMsgEngine() const
{
    return m_Engine;
}

ContactManager &App::getContactManager()
{
    if(!m_pContactManager)
        m_pContactManager = new ContactManager;
    return *m_pContactManager;
}

const ContactManager &App::getContactManager() const
{
    return const_cast<App*>(this)->getContactManager();
}

PopupManager &App::getPopupManager()
{
    if(!m_pPopupManager)
        m_pPopupManager = new PopupManager(getWindow());
    return *m_pPopupManager;
}

const PopupManager &App::getPopupManager() const
{
    return const_cast<App*>(this)->getPopupManager();
}

ThumbnailMaker &App::getThumbnailMaker()
{
    if(!m_pThumbnailMaker)
        m_pThumbnailMaker = new ThumbnailMaker(*this);
    return *m_pThumbnailMaker;
}

const ThumbnailMaker &App::getThumbnailMaker() const
{
    return const_cast<App*>(this)->getThumbnailMaker();
}

SystemSettingsManager &App::getSysSettingsManager()
{
    if(!m_pSysSettingsManager)
        m_pSysSettingsManager = new SystemSettingsManager;
    return *m_pSysSettingsManager;
}

const SystemSettingsManager &App::getSysSettingsManager() const
{
    return const_cast<App*>(this)->getSysSettingsManager();
}

void App::exit()
{
    terminate();
}
