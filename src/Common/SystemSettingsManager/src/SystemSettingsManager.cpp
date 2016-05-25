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
    : m_TelHandleList()
{
    int err = telephony_init(&m_TelHandleList);
    MSG_LOG("telephony_init = ", err);

    system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR, CALLBACK(onTimeFormatChanged), this);
    system_settings_set_changed_cb(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, CALLBACK(onLanguageChanged), this);
}

SystemSettingsManager::~SystemSettingsManager()
{
    telephony_deinit(&m_TelHandleList);
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

bool SystemSettingsManager::isSimInserted() const
{
    bool res = false;

    if(m_TelHandleList.count > 0)
    {
        telephony_h handle = m_TelHandleList.handle[0];
        telephony_sim_state_e simState = TELEPHONY_SIM_STATE_UNAVAILABLE;
        telephony_sim_get_state(handle, &simState);
        res = simState != TELEPHONY_SIM_STATE_UNAVAILABLE &&
              simState != TELEPHONY_SIM_STATE_UNKNOWN;
    }

    return res;
}

bool SystemSettingsManager::isMobileDataEnabled() const
{
    bool res = false;
    system_settings_get_value_bool(SYSTEM_SETTINGS_KEY_3G_DATA_NETWORK_ENABLED, &res);
    return res;
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

