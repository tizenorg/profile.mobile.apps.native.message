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
#include "MessageMmsPrivate.h"
#include "MsgUtils.h"
#include "MsgAddressPrivate.h"
#include "MsgConversationItemPrivate.h"
#include "Logger.h"
#include "MsgUtilsPrivate.h"

#include <msg_storage.h>

using namespace Msg;

MsgStoragePrivate::MsgStoragePrivate(msg_handle_t serviceHandle)
    : MsgStorage()
    , m_ServiceHandle(serviceHandle)
{
    TRACE;
    msg_reg_storage_change_callback(m_ServiceHandle, msg_storage_change_cb, this);
}

void MsgStoragePrivate::notifyListeners(const MsgIdList &idList, ListenerMethod method)
{
    for(auto listener: m_Listeners)
    {
        (listener->*method)(idList);
    }
}

void MsgStoragePrivate::msg_storage_change_cb(msg_handle_t handle, msg_storage_change_type_t storageChangeType, msg_id_list_s *pMsgIdList, void *user_param)
{
    TRACE;
    MsgStoragePrivate *self = static_cast<MsgStoragePrivate *>(user_param);
    MsgIdList msgIdList;

    int count = pMsgIdList->nCount;
    msgIdList.reserve(count);

    for(int i = 0; i < count; ++i)
    {
        msgIdList.push_back(pMsgIdList->msgIdList[i]);
    }

    self->notifyListeners(msgIdList, &IMsgStorageListener::onMsgStorageChange);

    switch(storageChangeType)
    {
        case MSG_STORAGE_CHANGE_UPDATE:
            self->notifyListeners(msgIdList, &IMsgStorageListener::onMsgStorageUpdate);
            break;
        case MSG_STORAGE_CHANGE_INSERT:
            self->notifyListeners(msgIdList, &IMsgStorageListener::onMsgStorageInsert);
            break;
        case MSG_STORAGE_CHANGE_DELETE:
            self->notifyListeners(msgIdList, &IMsgStorageListener::onMsgStorageDelete);
            break;
        case MSG_STORAGE_CHANGE_CONTACT:
            self->notifyListeners(msgIdList, &IMsgStorageListener::onMsgStorageContact);
            break;
    }
}

MsgStoragePrivate::~MsgStoragePrivate()
{
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

ThreadId MsgStoragePrivate::getThreadId(const std::list<std::string> &addressList)
{
    msg_thread_id_t tid = -1;
    msg_struct_t msgInfo = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);

    if(msgInfo)
    {
        for(auto &addr : addressList)
        {
            MsgAddress::AddressType type = MsgUtils::getAddressType(addr);
            msg_struct_t tmpAddr = nullptr;

            if (msg_list_add_item(msgInfo, MSG_MESSAGE_ADDR_LIST_HND, &tmpAddr) == MSG_SUCCESS)
            {
                msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_TYPE_INT, MsgUtilsPrivate::addressTypeToNative(type));
                msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_RECIPIENT_TYPE_INT, MSG_RECIPIENTS_TYPE_TO);
                MsgUtilsPrivate::setStr(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, addr);
            }
        }

        msg_list_handle_t addrList = nullptr;
        msg_get_list_handle(msgInfo, MSG_MESSAGE_ADDR_LIST_HND, (void **)&addrList);
        if(addrList)
            msg_get_thread_id_by_address2(m_ServiceHandle, addrList, &tid);

        msg_release_struct(&msgInfo);
    }

    return (ThreadId)tid;
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

bool MsgStoragePrivate::deleteThread(ThreadId id)
{
    return msg_delete_thread_message_list(m_ServiceHandle, id, true) == 0;
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

MessageRef MsgStoragePrivate::getMessage(MsgId id)
{
    MessageRef msgRef;
    msg_struct_t msg = nullptr;

    msg_struct_t sendOpt = msg_create_struct(MSG_STRUCT_SENDOPT);
    if(msg_get_message(m_ServiceHandle, (msg_message_id_t)id, msg, sendOpt) == 0)
    {
        int msgType = MSG_TYPE_INVALID;
        msg_get_int_value(msg, MSG_MESSAGE_TYPE_INT, &msgType);

        switch(msgType)
        {
            case MSG_TYPE_SMS:
                msgRef = std::make_shared<MessageSMSPrivate>(true, msg);
                break;

            case MSG_TYPE_MMS:
                msgRef = std::make_shared<MessageMmsPrivate>(true, msg);
                break;

            default:
                msg_release_struct(&msg);
                MSG_ASSERT(false, "Unsupported message type");
        }
    }
    msg_release_struct(&sendOpt);
    return msgRef;
}

MsgId MsgStoragePrivate::saveMessage(Message &msg)
{
    MsgId newMsgId;
    MessagePrivate &msgPriv = dynamic_cast<MessagePrivate&>(msg);

    msg_struct_t sendOpt = msg_create_struct(MSG_STRUCT_SENDOPT);
    msg_set_bool_value(sendOpt, MSG_SEND_OPT_SETTING_BOOL, false);

    msgPriv.commit();
    if(msgPriv.getId().isValid())
    {
        if(msg_update_message(m_ServiceHandle, msgPriv, sendOpt) == 0)
        {
            newMsgId = msg.getId();
        }
    }
    else
    {
        int tmpMsgId = msg_add_message(m_ServiceHandle, msgPriv, sendOpt);
        if(tmpMsgId > 0)
        {
            newMsgId = tmpMsgId;
        }
    }
    msg_release_struct(&sendOpt);

    return newMsgId;
}
