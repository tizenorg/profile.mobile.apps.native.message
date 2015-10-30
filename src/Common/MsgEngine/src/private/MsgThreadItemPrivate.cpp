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

const int defaultStrFiledLength = 128;

MsgThreadItemPrivate::MsgThreadItemPrivate(msg_handle_t serviceHandle, ThreadId threadId)
    : MsgThreadItem(threadId)
    , m_ServiceHandle(serviceHandle)
{

}

MsgThreadItemPrivate::~MsgThreadItemPrivate()
{

}

void MsgThreadItemPrivate::updateName(msg_struct_t threadInfo)
{
    if(threadInfo)
    {
        char threadData[128] = {0};
        msg_get_str_value(threadInfo, MSG_THREAD_NAME_STR, threadData, defaultStrFiledLength);
        MSG_LOG_INFO(m_Name);
        m_Name = std::string(threadData);
    }
}

void MsgThreadItemPrivate::updateLastMessage(msg_struct_t threadInfo)
{
    if(threadInfo)
    {
        char threadData[128] = {0};
        msg_get_str_value(threadInfo, MSG_THREAD_MSG_DATA_STR, threadData, defaultStrFiledLength);
        MSG_LOG_INFO(m_LastMessage);
        m_LastMessage = std::string(threadData);
    }
}

void MsgThreadItemPrivate::updateLastTime(msg_struct_t threadInfo)
{
    if(threadInfo)
    {
        int time = 0;
        msg_get_int_value(threadInfo, MSG_THREAD_MSG_TIME_INT, &time);
        MSG_LOG_INFO(m_Time);
        m_Time = time;
    }
}

void MsgThreadItemPrivate::update()
{
    msg_struct_t threadInfo = msg_create_struct(MSG_STRUCT_THREAD_INFO);
    int result = msg_get_thread(m_ServiceHandle, m_Id, threadInfo);
    if(result == MSG_SUCCESS)
    {
        updateName(threadInfo);
        updateLastMessage(threadInfo);
        updateLastTime(threadInfo);
        msg_release_struct(&threadInfo);
    }
    else
    {
        MSG_LOG_INFO(result);
    }
}

void MsgThreadItemPrivate::initConversationList(MsgConversationList &list) const
{
    msg_struct_list_s convList;
    int error = msg_get_conversation_view_list(m_ServiceHandle, m_Id, &convList);
    if(error == MSG_SUCCESS)
    {
        int convItemId = 0;
        for (int i = 0; i <= convList.nCount - 1; i++)
        {
            if(MSG_SUCCESS == msg_get_int_value(convList.msg_struct_info[i], MSG_CONV_MSG_ID_INT, &convItemId))
            {
                BaseMsgConversationItemRef ref(new MsgConversationItemPrivate(m_ServiceHandle, m_Id, convItemId));
                ref->update();
                list.push_back(ref);
            }
        }
        msg_release_list_struct(&convList);
    }
}

MsgConversationList MsgThreadItemPrivate::getConversationList() const
{
    MsgConversationList list;
    initConversationList(list);
    return list;
}
