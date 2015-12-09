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
#include "MsgUtilsPrivate.h"

#include <Logger.h>
#include <string>
#include <msg.h>
#include <msg_storage.h>

using namespace Msg;

MessagePrivate::MessagePrivate(bool release, msg_struct_t msgStruct)
    : Message()
    , MsgStructPrivate(release, msgStruct)
    , m_Address(false)
    , m_AddressList(false)
{
}

MessagePrivate::~MessagePrivate()
{
}

void MessagePrivate::set(msg_struct_t msgStruct)
{
    MsgStructPrivate::set(msgStruct);
}

const MsgAddressListHandlePrivate &MessagePrivate::getAddressList() const
{
    msg_list_handle_t addrList = nullptr;
    msg_get_list_handle(m_MsgStruct, MSG_MESSAGE_ADDR_LIST_HND, (void **)&addrList);
    m_AddressList.set(addrList);
    return m_AddressList;
}

MsgAddressPrivate &MessagePrivate::addAddress()
{
    msg_struct_t addr = nullptr;
    msg_list_add_item(m_MsgStruct, MSG_MESSAGE_ADDR_LIST_HND, &addr);
    m_Address.set(addr);
    return m_Address;
}

void MessagePrivate::addAddresses(const MsgAddressList &list)
{
    int len = list.getLength();
    for(int i = 0; i < len; ++i)
    {
        MsgAddressPrivate &newAddr = MessagePrivate::addAddress();
        newAddr.setFields(list[i]);
    }
}

MsgId MessagePrivate::getId() const
{
    int id = -1;
    msg_get_int_value(m_MsgStruct, MSG_MESSAGE_ID_INT, &id);
    return id;
}

void MessagePrivate::setId(MsgId id)
{
    msg_set_int_value(m_MsgStruct, MSG_MESSAGE_ID_INT, id);
}

ThreadId MessagePrivate::getThreadId() const
{
    int threadId = -1;
    msg_get_int_value(m_MsgStruct, MSG_MESSAGE_THREAD_ID_INT, &threadId);
    return threadId;
}

time_t MessagePrivate::getTime() const
{
    int time = 0;
    msg_get_int_value(m_MsgStruct, MSG_MESSAGE_DISPLAY_TIME_INT, &time);
    return time;
}

// FIXME: internal compiler error(arm only)
/*std::string MessagePrivate::getSubject() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MESSAGE_SUBJECT_STR, MAX_SUBJECT_LEN);
}

void MessagePrivate::setSubject(const std::string &text)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MESSAGE_SUBJECT_STR, text);
}*/

void MessagePrivate::commit()
{

}
