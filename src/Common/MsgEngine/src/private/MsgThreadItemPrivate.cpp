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

#include "MsgThreadItemPrivate.h"
#include "MsgConversationItemPrivate.h"
#include "MsgEngine.h"
#include "Logger.h"

#include <msg_storage.h>

using namespace Msg;

MsgThreadItemPrivate::MsgThreadItemPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , MsgThreadItem()
{

}

MsgThreadItemPrivate::~MsgThreadItemPrivate()
{

}

ThreadId MsgThreadItemPrivate::getId() const
{
    int id = -1;
    msg_get_int_value(m_MsgStruct, MSG_THREAD_ID_INT, &id);
    return id;
}

std::string MsgThreadItemPrivate::getName() const
{
    std::string res;
    char buf[MAX_DISPLAY_NAME_LEN + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_THREAD_NAME_STR, buf, MAX_DISPLAY_NAME_LEN) == 0)
    {
        res.assign(buf);
    }
    return res;
}

std::string MsgThreadItemPrivate::getLastMessage() const
{
    std::string res;
    char buf[MAX_MSG_TEXT_LEN + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_THREAD_MSG_DATA_STR, buf, MAX_MSG_TEXT_LEN) == 0)
    {
        res.assign(buf);
    }
    return res;
}

time_t MsgThreadItemPrivate::getTime() const
{
    int time = 0;
    msg_get_int_value(m_MsgStruct, MSG_THREAD_MSG_TIME_INT, &time);
    return time;
}

int MsgThreadItemPrivate::getStatus() const
{
    // TODO: impl
    return 0;
}

int MsgThreadItemPrivate::getUnreadCount() const
{
    int unread = 0;
    msg_get_int_value(m_MsgStruct, MSG_THREAD_UNREAD_COUNT_INT, &unread);
    return unread;
}

