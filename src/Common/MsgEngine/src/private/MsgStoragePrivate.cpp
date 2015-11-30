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

#include "MsgStoragePrivate.h"
#include "MsgThreadItemPrivate.h"
#include "MessageSMSPrivate.h"
#include "MsgUtils.h"
#include "MsgAddressPrivate.h"
#include "MsgConversationItemPrivate.h"
#include "Logger.h"

#include <msg_storage.h>

using namespace Msg;

MsgStoragePrivate::MsgStoragePrivate(msg_handle_t serviceHandle)
    : MsgStorage()
    , m_ServiceHandle(serviceHandle)
{
    TRACE;
    msg_reg_storage_change_callback(m_ServiceHandle, msg_storage_change_cb, this);
}

void MsgStoragePrivate::onStorageUpdate(MsgIdList msgIdList)
{
    //TODO
}

void MsgStoragePrivate::onStorageInsert(MsgIdList msgIdList)
{
    for(auto listener: m_Listeners)
    {
        listener->onMsgStorageChange();
    }
}

void MsgStoragePrivate::onStorageDelete(MsgIdList msgIdList)
{
    for(auto listener: m_Listeners)
    {
        listener->onMsgStorageChange();
    }
}

void MsgStoragePrivate::onStorageContact(MsgIdList msgIdList)
{
    //TODO
}


void MsgStoragePrivate::msg_storage_change_cb(msg_handle_t handle, msg_storage_change_type_t storageChangeType, msg_id_list_s *pMsgIdList, void *user_param)
{
    TRACE;
    MsgStoragePrivate *self = static_cast<MsgStoragePrivate *>(user_param);
    MsgIdList msgIdList;

    int count = pMsgIdList->nCount;
    for(int i = 0; i < count; i++)
    {
        msgIdList.push_back(pMsgIdList->msgIdList[i]);
    }

    switch(storageChangeType)
    {
        case MSG_STORAGE_CHANGE_UPDATE:
            self->onStorageUpdate(msgIdList);
            break;
        case MSG_STORAGE_CHANGE_INSERT:
            self->onStorageInsert(msgIdList);
            break;
        case MSG_STORAGE_CHANGE_DELETE:
            self->onStorageDelete(msgIdList);
            break;
        case MSG_STORAGE_CHANGE_CONTACT:
            self->onStorageContact(msgIdList);
            break;
    }
}

MsgStoragePrivate::~MsgStoragePrivate()
{
}

MessageSMS *MsgStoragePrivate::createSms()
{
    MessageSMSPrivate *sms = nullptr;
    msg_struct_t msgInfo = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);
    if(msgInfo)
    {
        sms = new MessageSMSPrivate(true, msgInfo);
    }
    return sms;
}

MessageRef MsgStoragePrivate::createMessage(Message::Type type)
{
    MessageRef msg;
    switch(type)
    {
        case Message::MT_SMS:
            msg.reset(createSms());
            break;

        case Message::MT_MMS:
            // TODO: impl
            break;

        default:
        case Message::MT_Unknown:
            break;
    }
    return msg;
}

MsgThreadListRef MsgStoragePrivate::getThreadList()
{
    MsgThreadListRef res;
    msg_struct_list_s msgList = {};
    msg_struct_t sortRule = msg_create_struct(MSG_STRUCT_SORT_RULE);
    msg_set_int_value(sortRule, MSG_SORT_RULE_SORT_TYPE_INT, MSG_SORT_BY_THREAD_DATE);
    msg_set_bool_value(sortRule, MSG_SORT_RULE_ACSCEND_BOOL, false);
    int error = msg_get_thread_view_list(m_ServiceHandle, sortRule, &msgList);
    msg_release_struct(&sortRule);

    if(error == 0)
    {
        res.reset(new MsgThreadStructListPrivate(true, msgList));
    }

    return res;
}

MessageSMSListRef MsgStoragePrivate::getSimMsgList()
{
    MessageSMSListRef res;

    msg_struct_list_s msgList;

    msg_struct_t listCond = msg_create_struct(MSG_STRUCT_MSG_LIST_CONDITION);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_FOLDER_ID_INT, MSG_ALLBOX_ID);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_MSGTYPE_INT, MSG_TYPE_SMS);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_SIM_INDEX_INT, 1);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_STORAGE_ID_INT, MSG_STORAGE_SIM);

    if(msg_get_message_list2(m_ServiceHandle, listCond, &msgList) == 0)
    {
        res.reset(new SmsStructListPrivate(true, msgList));
    }

    return res;
}

ThreadId MsgStoragePrivate::getThreadId(const MsgAddressList &addressList)
{
    msg_thread_id_t id = -1;

    if(const MsgAddressListHandlePrivate *privateList = dynamic_cast<const MsgAddressListHandlePrivate*>(&addressList))
    {
        msg_list_handle_t handleList = *privateList;
        msg_get_thread_id_by_address2(m_ServiceHandle, handleList, &id);
    }
    else if(const MsgAddressStructListPrivate *privateList = dynamic_cast<const MsgAddressStructListPrivate*>(&addressList))
    {
        msg_struct_list_s msgStructList = *privateList;
        msg_get_thread_id_by_address(m_ServiceHandle, &msgStructList, &id);
    }
    else
    {
        assert(false);
    }

    return (ThreadId)id;
}

MsgAddressListRef MsgStoragePrivate::getAddressList(ThreadId id)
{
    MsgAddressStructListPrivate *result = new MsgAddressStructListPrivate(true);
    msg_get_address_list(m_ServiceHandle, id, &result->get());
    return MsgAddressListRef(result);
}

MsgThreadItemRef MsgStoragePrivate::getThread(ThreadId id)
{
    MsgThreadItemRef res;
    msg_struct_t thread = msg_create_struct(MSG_STRUCT_THREAD_INFO);
    if(msg_get_thread(m_ServiceHandle, id, thread) == 0)
    {
        res.reset(new MsgThreadItemPrivate(true, thread));
    }
    return res;
}

int MsgStoragePrivate::deleteThread(ThreadId id)
{
    int result = msg_delete_thread_message_list(m_ServiceHandle, id, true);
    MSG_LOG_INFO("Msg storage delete thread error = ", result);
    return result;
}

MsgConversationListRef MsgStoragePrivate::getConversationList(ThreadId id)
{
    MsgConversationListRef res;
    msg_struct_list_s convList = {};
    int error = msg_get_conversation_view_list(m_ServiceHandle, id, &convList);

    if(error == 0)
    {
        res.reset(new MsgConversationStructListPrivate(true, convList));
    }
    return res;
}
