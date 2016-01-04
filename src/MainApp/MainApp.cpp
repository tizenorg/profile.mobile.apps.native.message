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

#include "MainApp.h"
#include "StandardWindow.h"
#include "NaviFrameController.h"
#include "CallbackAssist.h"
#include "Logger.h"
#include "AppControlParser.h"
#include "PathUtils.h"
#include "Config.h"

#include <assert.h>
#include <stdlib.h>
#include <system_settings.h>

using namespace Msg;

MainApp::MainApp()
    : m_pWindow(nullptr)
    , m_pRootController(nullptr)
{

}

MainApp::~MainApp()
{

}

int MainApp::start(int argc, char *argv[])
{
    ui_app_lifecycle_callback_s cbs = {};

    cbs.app_control = [](app_control_h app_control, void *user_data)
    {
        static_cast<MainApp*>(user_data)->onAppControl(app_control);
    };

    cbs.create = [](void *user_data)->bool
    {
        return static_cast<MainApp*>(user_data)->onAppCreate();
    };

    cbs.pause = [](void *user_data)
    {
        static_cast<MainApp*>(user_data)->onAppPause();
    };

    cbs.resume = [](void *user_data)
    {
        static_cast<MainApp*>(user_data)->onAppResume();
    };

    cbs.terminate = [](void *user_data)
    {
        static_cast<MainApp*>(user_data)->onAppTerminate();
    };

    app_event_handler_h handlers[3] = {};
    ui_app_add_event_handler(&handlers[0], APP_EVENT_LOW_BATTERY, APP_EVENT_CALLBACK(MainApp, onLowBattery), this);
    ui_app_add_event_handler(&handlers[1], APP_EVENT_LANGUAGE_CHANGED, APP_EVENT_CALLBACK(MainApp, onLanguageChanged), this);
    ui_app_add_event_handler(&handlers[2], APP_EVENT_REGION_FORMAT_CHANGED, APP_EVENT_CALLBACK(MainApp, onRegionChanged), this);

    return ui_app_main(argc, argv, &cbs, this);
}


Window &MainApp::getWindow()
{
    MSG_ASSERT(m_pWindow, "Window is null");
    return *m_pWindow;
}

const Window &MainApp::getWindow() const
{
    MSG_ASSERT(m_pWindow, "Window is null");
    return *m_pWindow;
}


void MainApp::terminate()
{
   /* if(m_pWindow)
        m_pWindow->lower();*/

    ui_app_exit();
}

void MainApp::initThemes()
{
    std::string imagesPath = PathUtils::getResourcePath(IMAGES_EDJ_PATH);
    elm_theme_extension_add(nullptr, imagesPath.c_str());

    std::string bubbleThemePath = PathUtils::getResourcePath(BUBBLE_THEME_EDJ_PATH);
    elm_theme_extension_add(nullptr, bubbleThemePath.c_str());

    std::string buttonThemePath = PathUtils::getResourcePath(BUTTON_THEME_EDJ_PATH);
    elm_theme_extension_add(nullptr, buttonThemePath.c_str());
}

bool MainApp::onAppCreate()
{
    TRACE;
    bindtextdomain(PROJECT_NAME, LOCALEDIR);
    elm_app_base_scale_set(1.8); // If the application is made based on HD(720x1280), set 2.4.
    elm_config_preferred_engine_set("opengl_x11");

    initThemes();

    m_pWindow = new StandardWindow;
    m_pWindow->show();

    m_pRootController = new NaviFrameController(*this);
    m_pWindow->setContent(*m_pRootController);

    return true;
}

void MainApp::onAppTerminate()
{
    TRACE;
    m_pRootController->destroy();
    m_pWindow->destroy();
}

void MainApp::onAppPause()
{
    TRACE;
}

void MainApp::onAppResume()
{
    TRACE;
}

void MainApp::onAppControl(app_control_h app_control)
{
    TRACE;

    AppControlCommandRef cmd = AppControlParser::parse(app_control);

    if(m_pRootController)
        m_pRootController->executeCommand(cmd);
}

void MainApp::onLanguageChanged(app_event_info_h appEvent)
{
    TRACE;

    char *languageSet = nullptr;
    system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &languageSet);

    if(languageSet)
    {
        MSG_LOG("Language: ", languageSet);
        elm_language_set(languageSet);
        free(languageSet);
    }
}

void MainApp::onLowBattery(app_event_info_h appEvent)
{
    TRACE;
}
void MainApp::onRegionChanged(app_event_info_h appEvent)
{
    TRACE;
}

