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
#include <unordered_map>

#include "AppControlUtils.h"

using namespace Msg;

namespace
{
    typedef std::unordered_map<std::string, AppControlCompose::OpComposeType> OperationMap;

    const OperationMap operationMap =
    {
        {APP_CONTROL_OPERATION_COMPOSE, AppControlCompose::OpCompose},
        {APP_CONTROL_OPERATION_SHARE, AppControlCompose::OpShare},
        {APP_CONTROL_OPERATION_MULTI_SHARE, AppControlCompose::OpMultiShare},
        {APP_CONTROL_OPERATION_SHARE_TEXT, AppControlCompose::OpShareText}
    };

    AppControlCompose::OpComposeType getOperation(const std::string &op)
    {
        auto it = operationMap.find(op);
        return it != operationMap.end() ? it->second : AppControlCompose::OpUnknown;
    };
}

AppControlCompose::AppControlCompose(const std::string &opMsg, app_control_h handle)
    : AppControlCommand(opMsg, OpComposeFamily)
    , m_ComposeType(getOperation(opMsg))
    , m_isMms(false)
{
    if(handle)
    {
        switch(m_ComposeType)
        {
            case OpCompose:
                createComposeOp(handle);
                break;
            case OpShare:
                createShareOp(handle);
                break;
            case OpMultiShare:
                createMultiShareOp(handle);
                break;
            case OpShareText:
                createShareTextOp(handle);
                break;
            case OpUnknown:
                MSG_LOG_ERROR("Invalid op-type")
                break;
        }
    }
}

AppControlCompose::~AppControlCompose()
{
}

void AppControlCompose::createComposeOp(app_control_h handle)
{
    parseUriCompose(handle);
    if (m_RecipientList.empty())
    {
        AppControlUtils::getExtraDataArray(handle, APP_CONTROL_DATA_TO, m_RecipientList);
    }
    m_MessageText = AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_TEXT);
    m_Subject = AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_SUBJECT);
    AppControlUtils::getExtraDataArray(handle, APP_CONTROL_DATA_PATH, m_FileList);
}
void AppControlCompose::createShareOp(app_control_h handle)
{
    parseUriShare(handle);
    if (m_FileList.empty())
    {
        m_FileList.push_back(AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_PATH));
    }
}
void AppControlCompose::createMultiShareOp(app_control_h handle)
{
    AppControlUtils::getExtraDataArray(handle, APP_CONTROL_DATA_PATH, m_FileList);
    parseUriShare(handle);
}
void AppControlCompose::createShareTextOp(app_control_h handle)
{
    parseUriShare(handle);
    m_MessageText = AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_TEXT);
    m_Subject = AppControlUtils::getExtraData(handle, APP_CONTROL_DATA_SUBJECT);
    AppControlUtils::getExtraDataArray(handle, APP_CONTROL_DATA_PATH, m_FileList);
}

AppControlCompose::OpComposeType AppControlCompose::getComposeType() const
{
    return m_ComposeType;
}

const AppControlCompose::RecipientList &AppControlCompose::getRecipientList() const
{
    return m_RecipientList;
}

bool AppControlCompose::isMms() const
{
    return m_isMms;
}
const std::string &AppControlCompose::getMessageText() const
{
    return m_MessageText;
}
const std::string &AppControlCompose::getMessageSubject() const
{
    return m_Subject;
}
const AppControlCompose::FileList &AppControlCompose::getFileList() const
{
    return m_FileList;
}

bool AppControlCompose::parseUriCompose(app_control_h handle)
{
    TRACE;
    bool res = false;
    char *uri = nullptr;

    if (APP_CONTROL_ERROR_NONE == app_control_get_uri(handle, &uri))
    {
        if (uri)
        {
            MSG_LOG("uri = ", uri);
            std::string uriToParse(uri);
            std::istringstream is(uriToParse);

            std::string cur;
            std::getline(is, cur, ':');
            MSG_LOG("cur = ", cur.c_str());

            if (cur == "sms" || cur == "mmsto")
            {
                m_isMms = (cur == "mmsto");
                for (; std::getline(is, cur, ',');)
                {
                    m_RecipientList.push_back(cur);
                }
                res = true;
            }
        }
        free(uri);
    }

    return res;
}

bool AppControlCompose::parseUriShare(app_control_h handle)
{
    TRACE;
    bool res = false;
    char *uri = nullptr;

    if (APP_CONTROL_ERROR_NONE == app_control_get_uri(handle, &uri))
    {
        if (uri)
        {
            MSG_LOG("uri = ", uri);
            std::string uriToParse(uri);
            std::istringstream is(uriToParse);

            std::string cur;
            std::getline(is, cur, ':');
            MSG_LOG("cur = ", cur.c_str());

            if (cur == "sms" || cur == "mmsto" || cur == "file")
            {
                m_isMms = (cur == "mmsto" || cur == "file");
                if (cur == "file" && m_FileList.empty())
                {
                    std::string prefix("file://");
                    m_FileList.push_back(uriToParse.erase(0, prefix.length()));
                }
                res = true;
            }
        }
        free(uri);
    }

    return res;
}

