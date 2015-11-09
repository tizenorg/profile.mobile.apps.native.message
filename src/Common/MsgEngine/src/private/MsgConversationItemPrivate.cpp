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
#include "MsgUtilsPrivate.h"

#include <msg_storage.h>
#include <assert.h>

using namespace Msg;

MsgConversationItemPrivate::MsgConversationItemPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
{

}

MsgConversationItemPrivate::~MsgConversationItemPrivate()
{

}

ConvItemId MsgConversationItemPrivate::getId() const
{
    int id = -1;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_ID_INT, &id);
    return id;
}

ThreadId MsgConversationItemPrivate::getThreadId() const
{
    int id = -1;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_THREAD_ID_INT, &id);
    return id;
}

std::string MsgConversationItemPrivate::getText() const
{
    std::string text;
    char buf[MAX_MSG_TEXT_LEN + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_TEXT_STR, buf, MAX_MSG_TEXT_LEN) == 0)
    {
        text.assign(buf);
    }
    return text;
}

time_t MsgConversationItemPrivate::getTime() const
{
    int time = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_DISPLAY_TIME_INT, &time);
    return time;
}

Message::Direction MsgConversationItemPrivate::getDirection() const
{
    int direction = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_DIRECTION_INT, &direction);
    return MsgUtilsPrivate::nativeToDirection(direction);
}
