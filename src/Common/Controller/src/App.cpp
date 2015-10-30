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

using namespace Msg;

App::App()
    : m_pContactManager(nullptr)
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
    delete m_pContactManager;
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

void App::exit()
{
    terminate();
}
