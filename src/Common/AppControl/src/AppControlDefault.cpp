/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "AppControlDefault.h"

using namespace Msg;

namespace
{
    const char *notificationPanel = "http://tizen.org/appcontrol/data/notification";
    const char *keyType = "type";
    const char *valueReply = "reply";
    const char *valueNewMsg = "new_msg";
    const char *valueSendFailed = "send_failed_msg";
    const char *keyMsgId = "msgId";
}

AppControlDefault::AppControlDefault(const std::string &opMsg, app_control_h handle)
    : AppControlCommand(opMsg, OpDefault)
    , m_MsgId()
    , m_DefaultType(UnknownType)
{
    parse(handle);
}

AppControlDefault::~AppControlDefault()
{

}

MsgId AppControlDefault::getMessageId() const
{
    return m_MsgId;
}

AppControlDefault::DefaultType AppControlDefault::getDefaultType() const
{
    return m_DefaultType;
}

void AppControlDefault::parse(app_control_h handle)
{
    char *key = nullptr;
    app_control_get_extra_data(handle, keyType, &key);
    if(!key)
    {
        m_DefaultType = MainType;
        return;
    }

    std::string type = key;
    free(key);
    if(type == valueReply)
        m_DefaultType = ReplyType;
    else if(type == valueNewMsg || type == valueSendFailed)
        viewMessage(handle);
    else
        MSG_LOG("Unknown type!");

    char *msgIdStr = nullptr;
    app_control_get_extra_data(handle, keyMsgId, &msgIdStr);
    if(!msgIdStr)
        return;

    m_MsgId = atoi(msgIdStr);
    free(msgIdStr);
}

bool AppControlDefault::isNotificationPanel(app_control_h handle) const
{
    char *keyVal = nullptr;
    app_control_get_extra_data(handle, notificationPanel, &keyVal);
    if(keyVal)
    {
        free(keyVal);
        return true;
    }
    else
        return false;
}

void AppControlDefault::viewMessage(app_control_h handle)
{
    if(isNotificationPanel(handle))
        m_DefaultType = NotificationType;
    else
        m_DefaultType = ViewType;
}
