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

#include "AppControlNewMessage.h"

using namespace Msg;

namespace
{
    const char *notificationPanel = "http://tizen.org/appcontrol/data/notification";
    const char *keyType = "type";
    const char *valueReply = "reply";
    const char *valueNewMsg = "new_msg";
    const char *keyMsgId = "msgId";
}

AppControlNewMessage::AppControlNewMessage(const std::string &opMsg, app_control_h handle)
    : AppControlCommand(opMsg, OpNewMessage)
    , m_MsgId()
    , m_NewMsgType(UnknownType)
{
    parse(handle);
}

AppControlNewMessage::~AppControlNewMessage()
{

}

MsgId AppControlNewMessage::getMessageId() const
{
    return m_MsgId;
}

AppControlNewMessage::NewMessageType AppControlNewMessage::getNewMessageType() const
{
    return m_NewMsgType;
}

void AppControlNewMessage::parse(app_control_h handle)
{
    char *key = nullptr;
    app_control_get_extra_data(handle, keyType, &key);
    if(!key)
        return;

    std::string keyType = key;
    free(key);
    if(keyType == valueReply)
        m_NewMsgType = ReplyType;
    else if(keyType == valueNewMsg)
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

bool AppControlNewMessage::isNotificationPanel(app_control_h handle) const
{
    char *keyVal = nullptr;
    app_control_get_extra_data(handle, notificationPanel, &keyVal);
    return keyVal != nullptr;
}

void AppControlNewMessage::viewMessage(app_control_h handle)
{
    if(isNotificationPanel(handle))
        m_NewMsgType = NotificationType;
    else
        m_NewMsgType = NewMsgType;
}
