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

using namespace Msg;

AppControlCompose::AppControlCompose(const std::string &opMsg)
    : AppControlCommand(opMsg, OpCompose)
    , m_MessageType(UnknownType)
{
}

AppControlCompose::~AppControlCompose()
{
}

bool AppControlCompose::initialize(app_control_h handle)
{
    TRACE;
    bool res = false;
    if(handle)
    {
        char *uri = NULL;
        if(APP_CONTROL_ERROR_NONE == app_control_get_uri(handle, &uri))
        {
            parseUri(uri);
            free(uri);
        }

        int arrayLength = 0;
        char **pArrayVal = NULL;
        if(APP_CONTROL_ERROR_NONE ==
                app_control_get_extra_data_array(handle, APP_CONTROL_DATA_TO, &pArrayVal, &arrayLength))
        {
            std::copy(pArrayVal, pArrayVal + arrayLength, std::back_inserter(m_RecipientList));
            free(pArrayVal);
        }

        char *text = NULL;
        if(APP_CONTROL_ERROR_NONE == app_control_get_extra_data(handle, APP_CONTROL_DATA_TEXT, &text))
        {
            m_messageText = text;
            free(text);
        }

        char *subj = NULL;
        if(APP_CONTROL_ERROR_NONE == app_control_get_extra_data(handle, APP_CONTROL_DATA_SUBJECT, &subj))
        {
            m_subject = subj;
            free(subj);
        }

        char **dataPath = NULL;
        if(APP_CONTROL_ERROR_NONE ==
                app_control_get_extra_data_array(handle, APP_CONTROL_DATA_PATH, &dataPath, &arrayLength))
        {
            std::copy(pArrayVal, dataPath + arrayLength, std::back_inserter(m_FileList));
            free(dataPath);
        }
    }
    return res;
}

bool AppControlCompose::parseUri(const char* uri)
{
    TRACE;
    bool res = false;
    if(uri)
    {
        MSG_LOG("uri = ", uri);
        std::string uriToParse(uri);
        std::istringstream is(uriToParse);

        std::string cur;
        std::getline(is, cur, '?')
        {
            MSG_LOG("cur = ", cur.c_str());
            if(cur == "sms")
            {
                m_MessageType = SmsType;
            }
            else if(cur == "mmsto")
            {
                m_MessageType = MmsType
            }
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
    return m_messageText;
}
const std::string AppControlCompose::getMessageSubject() const
{
    return m_subject;
}
const AppControlCompose::FileList &AppControlCompose::getFileList() const
{
    return m_FileList;
}
