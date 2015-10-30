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

#include "MsgConversationItemPrivate.h"
#include "Logger.h"
#include <msg_storage.h>

using namespace Msg;

const int defaultStrFiledLength = 128;

MsgConversationItemPrivate::MsgConversationItemPrivate(msg_handle_t serviceHandle, ThreadId threadId, ConvItemId convItemId)
    : MsgConversationItem(threadId, convItemId)
    , m_ServiceHandle(serviceHandle)
{

}

MsgConversationItemPrivate::~MsgConversationItemPrivate()
{

}

void MsgConversationItemPrivate::update()
{
    msg_struct_t convInfo = msg_create_struct(MSG_STRUCT_CONV_INFO);
    int result = msg_get_conversation(m_ServiceHandle, m_ItemId, convInfo);
    if(result == MSG_SUCCESS)
    {
        updateText(convInfo);
        updateTime(convInfo);
        updateDirection(convInfo);
        msg_release_struct(&convInfo);
    }
    else
    {
        MSG_LOG_INFO(result);
    }
}

void MsgConversationItemPrivate::updateText(msg_struct_t convItemInfo)
{
    if(convItemInfo)
    {
        char convItemText[defaultStrFiledLength] = {0};
        msg_get_str_value(convItemInfo, MSG_CONV_MSG_TEXT_STR, convItemText, defaultStrFiledLength);

        m_MsgText = std::string(convItemText);
        MSG_LOG_INFO(m_MsgText);
    }
}

void MsgConversationItemPrivate::updateTime(msg_struct_t convItemInfo)
{
    if(convItemInfo)
    {
        int time = 0;
        msg_get_int_value(convItemInfo, MSG_CONV_MSG_DISPLAY_TIME_INT, &time);

        m_Time = time;
        MSG_LOG_INFO(m_Time);
    }
}

void MsgConversationItemPrivate::updateDirection(msg_struct_t convItemInfo)
{
    if(convItemInfo)
    {
        int direction = 0;
        msg_get_int_value(convItemInfo, MSG_CONV_MSG_DIRECTION_INT, &direction);

        m_Direction = (Message::Direction)direction;
        MSG_LOG_INFO(m_Direction);
    }
}
