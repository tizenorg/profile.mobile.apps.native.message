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

#include "Config.h"
#include "MsgSettings.h"
#include <algorithm>

using namespace Msg;

const int defaultSmsGsm7MaxLen = 160;
const int defaultSmsMaxPage = 3;
const int defaultMaxAttachmentsCount = 10;

MsgSettings::MsgSettings()
{

}

MsgSettings::~MsgSettings()
{

}

int MsgSettings::getMessageTextMaxChar() const
{
    return defaultSmsGsm7MaxLen;
}

int MsgSettings::getMessageMaxPage() const
{
    return defaultSmsMaxPage;
}

int MsgSettings::getAttachmentsMaxCount() const
{
    return defaultMaxAttachmentsCount;
}

void MsgSettings::addListener(IMsgSettingsListener &listener)
{
    auto itr = std::find(m_Listeners.begin(), m_Listeners.end(), &listener);
    if(itr == m_Listeners.end())
    {
        m_Listeners.push_back(&listener);
    }
}

void MsgSettings::removeListener(IMsgSettingsListener &listener)
{
    auto itr = std::find(m_Listeners.begin(), m_Listeners.end(), &listener);
    if(itr != m_Listeners.end())
    {
        m_Listeners.erase(itr);
    }
}

