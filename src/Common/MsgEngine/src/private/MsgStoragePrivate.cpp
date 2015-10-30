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
#include "MsgDataContainer.h"
#include "MsgUtils.h"

#include "Logger.h"

using namespace Msg;

const int addressMaxLength = 254;

MsgStoragePrivate::MsgStoragePrivate(msg_handle_t serviceHandle)
    : MsgStorage()
    , m_ServiceHandleImpl(serviceHandle)
{
    TRACE;
    msg_reg_storage_change_callback(m_ServiceHandleImpl, msg_storage_change_cb, this);
}

void MsgStoragePrivate::onStorageChange()
{
    for(auto listener: m_Listeners)
    {
        listener->onMsgStorageChange();
    }
}

void MsgStoragePrivate::msg_storage_change_cb(msg_handle_t handle, msg_storage_change_type_t storageChangeType, msg_id_list_s *pMsgIdList, void *user_param)
{
    TRACE;
    MsgStoragePrivate *self = static_cast<MsgStoragePrivate *>(user_param);
    switch(storageChangeType)
    {
    //TODO : Implement appropriate functionality
        default:
            self->onStorageChange();
        break;
    }
}

MsgStoragePrivate::~MsgStoragePrivate()
{
}

MessageRef MsgStoragePrivate::createMessage(const MsgDataContainer & dataContainer)
{
    MessageRef msg;
    if(dataContainer.getType() == Message::MT_SMS)
    {
        MessageSMSPrivate *privSms = new MessageSMSPrivate(m_ServiceHandleImpl);
        privSms->setText(dataContainer.getText());
        privSms->setAddressList(dataContainer.getAddressList());
        msg.reset(privSms);
    }
    return msg;
}

MsgThreadList MsgStoragePrivate::getThreadList() const
{
    MsgThreadList list;
    initThreadList(list);
    return list;
}

MessageRefList MsgStoragePrivate::getSimMsgList() const
{
    MessageRefList list;
    initSimMsgList(list);
    return list;
}

ThreadId MsgStoragePrivate::getThreadId(const AddressList &addressList) const
{
    ThreadId id = 0;

    msg_struct_t msgInfo = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);

    if(msgInfo)
    {
        for(auto & address : addressList)
        {
            MsgAddress addrType = MsgUtils::getAddressType(address);
            _MSG_ADDRESS_TYPE_E tizenAddrType = MSG_ADDRESS_TYPE_UNKNOWN;

            switch(addrType)
            {
                case MsgAddress::Number:
                    tizenAddrType = MSG_ADDRESS_TYPE_PLMN;
                    break;

                case MsgAddress::Email:
                    tizenAddrType = MSG_ADDRESS_TYPE_EMAIL;
                    break;

                case MsgAddress::Invalid:
                default:
                    MSG_LOG_ERROR("Invalid address type");
                    break;
            }

            msg_struct_t tmpAddr = nullptr;
            if(msg_list_add_item(msgInfo, MSG_MESSAGE_ADDR_LIST_HND, &tmpAddr) == MSG_SUCCESS)
            {
                msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_TYPE_INT, tizenAddrType);
                msg_set_int_value(tmpAddr, MSG_ADDRESS_INFO_RECIPIENT_TYPE_INT, MSG_RECIPIENTS_TYPE_TO);
                msg_set_str_value(tmpAddr, MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, (char*)address.c_str(), address.size());
            }
        }
        msg_list_handle_t addrList = nullptr;
        msg_get_list_handle(msgInfo, MSG_MESSAGE_ADDR_LIST_HND, (void **)&addrList);

        if(addrList)
        {
            msg_get_thread_id_by_address2(m_ServiceHandleImpl, addrList, (msg_thread_id_t*)&id);
        }
        else
        {
            MSG_LOG_ERROR("addrList is null");
        }
        msg_release_struct(&msgInfo);
    }

    return id;
}

AddressList MsgStoragePrivate::getAddressListByThreadId(ThreadId id)
{
    AddressList result;
    msg_struct_list_s msgAddressList;
    int error = msg_get_address_list(m_ServiceHandleImpl, id, &msgAddressList);
    if(error == MSG_SUCCESS)
    {
        char addrValue[addressMaxLength + 1] = {0,};
        for (int i = 0; i < msgAddressList.nCount; i++)
        {
            msg_get_str_value(msgAddressList.msg_struct_info[i], MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, addrValue, addressMaxLength);
            result.push_back(std::string(addrValue));
        }
    }

    return result;
}

BaseMsgThreadItemRef MsgStoragePrivate::getThread(ThreadId id) const
{
    BaseMsgThreadItemRef ref(new MsgThreadItemPrivate(m_ServiceHandleImpl, id));
    ref->update();
    return ref;
}

void MsgStoragePrivate::initThreadList(MsgThreadList &list) const
{
    TRACE;
    msg_struct_list_s msgList;
    msg_struct_t sortRule = msg_create_struct(MSG_STRUCT_SORT_RULE);
    msg_set_int_value(sortRule, MSG_SORT_RULE_SORT_TYPE_INT, MSG_SORT_BY_THREAD_DATE);
    msg_set_bool_value(sortRule, MSG_SORT_RULE_ACSCEND_BOOL, false);
    msg_get_thread_view_list(m_ServiceHandleImpl, sortRule, &msgList);
    msg_release_struct(&sortRule);

    int threadId = 0;
    for (int i = 0; i <= msgList.nCount - 1; ++i)
    {
        if(MSG_SUCCESS == msg_get_int_value(msgList.msg_struct_info[i], MSG_THREAD_ID_INT, &threadId))
        {
            list.push_back(getThread(threadId));
        }
    }
    msg_release_list_struct(&msgList);
}

void MsgStoragePrivate::initSimMsgList(MessageRefList &list) const
{
    TRACE;
    msg_struct_list_s msgList;
    msg_struct_t listCond = msg_create_struct(MSG_STRUCT_MSG_LIST_CONDITION);

    msg_set_int_value(listCond, MSG_LIST_CONDITION_FOLDER_ID_INT, MSG_ALLBOX_ID);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_MSGTYPE_INT, MSG_TYPE_SMS);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_SIM_INDEX_INT, 1);
    msg_set_int_value(listCond, MSG_LIST_CONDITION_STORAGE_ID_INT, MSG_STORAGE_SIM);

    msg_get_message_list2(m_ServiceHandleImpl, listCond, &msgList);
    msg_release_struct(&listCond);

    int id = 0;
    for (int i = 0; i < msgList.nCount; ++i)
    {
        if(MSG_SUCCESS == msg_get_int_value(msgList.msg_struct_info[i], MSG_MESSAGE_ID_INT, &id))
        {
            MessageSMSPrivate *sms = new MessageSMSPrivate(m_ServiceHandleImpl, msgList.msg_struct_info[i]);
            MessageRef msg;

            msg.reset(sms);
            list.push_back(msg);
        }
    }
//    msg_release_list_struct(&msgList);
}

int MsgStoragePrivate::deleteThread(ThreadId id)
{
    int result = msg_delete_thread_message_list(m_ServiceHandleImpl, id, true);
    MSG_LOG_INFO("Msg storage delete thread error = ", result);
    return result;
}
