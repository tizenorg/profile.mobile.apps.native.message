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

#include "MessagePrivate.h"
#include "MsgEngine.h"

#include <Logger.h>
#include <string>
#include <msg.h>
#include <msg_storage.h>

using namespace Msg;

#define CHECK_MSG_STRUCT MSG_ASSERT(getMsgStruct(), "Handle is null!");

#define DEF_THREAD_ADDR_LEN 200

MessagePrivate::MessagePrivate(msg_handle_t handle)
    : m_pMsgStruct()
    , m_pHandle(handle)
{

}

MessagePrivate::~MessagePrivate()
{
    msg_release_struct(&m_pMsgStruct);
    m_pMsgStruct = nullptr;
}

void MessagePrivate::setAddressList(const AddressList &addressList)
{
    CHECK_MSG_STRUCT;

    for(auto addressItem : addressList)
    {
        msg_struct_t tmpAddr = nullptr;
        msg_list_add_item(m_pMsgStruct, MSG_MESSAGE_ADDR_LIST_HND, &tmpAddr);
        msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_TYPE_INT, MSG_ADDRESS_TYPE_PLMN);

        msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_TYPE_INT, MSG_ADDRESS_TYPE_PLMN);
        msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_RECIPIENT_TYPE_INT, MSG_RECIPIENTS_TYPE_TO);
        msg_set_str_value(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, const_cast<char *>(addressItem.c_str()), addressItem.length());
    }
}

AddressList MessagePrivate::getAddressList() const
{
    CHECK_MSG_STRUCT;

    AddressList addressList;
    msg_list_handle_t addrList = nullptr;

    msg_get_list_handle(m_pMsgStruct, MSG_MESSAGE_ADDR_LIST_HND, (void **)&addrList);

    return addressList;
}

ThreadId MessagePrivate::getThreadId() const
{
    ThreadId id = 0;

    // TODO:

    return id;
}

void MessagePrivate::setMsgStruct(msg_struct_t msgStruct)
{
    m_pMsgStruct = msgStruct;
}

time_t MessagePrivate::getTime() const
{
    time_t time;
    msg_get_int_value(getMsgStruct(), MSG_MESSAGE_DISPLAY_TIME_INT, time);
    return time;
}

std::string MessagePrivate::getNumber() const
{
    std::string strNumber;
    char buf[DEF_THREAD_ADDR_LEN] = {0};
    msg_get_str_value(getMsgStruct(), MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, buf, DEF_THREAD_ADDR_LEN);
    strNumber.assign(buf);
    return strNumber;
}
