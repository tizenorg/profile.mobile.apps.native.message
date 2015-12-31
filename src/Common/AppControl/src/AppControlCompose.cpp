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

#include "AppControlCompose.h"
#include "Logger.h"
#include <algorithm>

#include "AppControlUtils.h"

using namespace Msg;

AppControlCompose::AppControlCompose(const std::string &opMsg, app_control_h handle)
    : AppControlCommand(opMsg, OpCompose)
{
    if(handle)
    {
        char *uri = NULL;
        if(APP_CONTROL_ERROR_NONE == app_control_get_uri(handle, &uri))
        {
            parseUri(uri);
            free(uri);
        }

        AppControlUtils::getExtraDataArray(handle, APP_CONTROL_DATA_TO, m_RecipientList);
        m_MessageText = AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_TEXT);
        m_Subject = AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_SUBJECT);
        AppControlUtils::getExtraDataArray(handle, APP_CONTROL_DATA_PATH, m_FileList);
    }
}

AppControlCompose::~AppControlCompose()
{
}

bool AppControlCompose::parseUri(const char *uri)
{
    TRACE;
    bool res = false;
    if(uri)
    {
        MSG_LOG("uri = ", uri);
        std::string uriToParse(uri);
        std::istringstream is(uriToParse);

        std::string cur;
        std::getline(is, cur, '?');
        MSG_LOG("cur = ", cur.c_str());
        if(cur == "sms")
        {
             m_MessageType = SmsType;
        }
        else if(cur == "mmsto")
        {
            m_MessageType = MmsType;
        }

        if(m_MessageType != UnknownType)
        {
            //TODO: further uri parsing
            res = true;
        }

    }
    return res;
}

const AppControlCompose::RecipientList &AppControlCompose::getRecipientList() const
{
    return m_RecipientList;
}

AppControlCompose::MessageType AppControlCompose::getMessageType() const
{
    return m_MessageType;
}
const std::string AppControlCompose::getMessageText() const
{
    return m_MessageText;
}
const std::string AppControlCompose::getMessageSubject() const
{
    return m_Subject;
}
const AppControlCompose::FileList &AppControlCompose::getFileList() const
{
    return m_FileList;
}
