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
 */

#include "SystemSettingsManager.h"
#include "Logger.h"

#include <algorithm>
#include <system_settings.h>

using namespace Msg;


#define CALLBACK(method) [](system_settings_key_e key, void *user_data) \
{                                                                       \
    static_cast<SystemSettingsManager*>(user_data)->method();           \
}

SystemSettingsManager::SystemSettingsManager()
{
    system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR, CALLBACK(onTimeFormatChanged), this);
    system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, CALLBACK(onLanguageChanged), this);
}

void SystemSettingsManager::addListener(ISystemSettingsManager &l)
{
    auto it = std::find(m_Listeners.begin(), m_Listeners.end(), &l);
    if(it == m_Listeners.end())
        m_Listeners.push_back(&l);
}

void SystemSettingsManager::removeListener(ISystemSettingsManager&l)
{
    auto it = std::find(m_Listeners.begin(), m_Listeners.end(), &l);
    if(it != m_Listeners.end())
        m_Listeners.erase(it);
}

void SystemSettingsManager::onTimeFormatChanged()
{
    for(ISystemSettingsManager *it : m_Listeners)
    {
        it->onTimeFormatChanged();
    }
}

void SystemSettingsManager::onLanguageChanged()
{
    for(ISystemSettingsManager *it : m_Listeners)
    {
        it->onLanguageChanged();
    }
}
