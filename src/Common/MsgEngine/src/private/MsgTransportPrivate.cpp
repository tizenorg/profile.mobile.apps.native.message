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

#include "MsgTransportPrivate.h"
#include "MessagePrivate.h"
#include "Logger.h"

using namespace Msg;

MsgTransportPrivate::MsgTransportPrivate(msg_handle_t serviceHandle)
    : MsgTransport()
    , m_ServiceHandle(serviceHandle)
{

}

MsgTransportPrivate::~MsgTransportPrivate()
{

}

MsgTransport::SendResult MsgTransportPrivate::sendMessage(Message &msg, ThreadId *threadId)
{
    msg_struct_t req = msg_create_struct(MSG_STRUCT_REQUEST_INFO);
    int err = MSG_SUCCESS;

    msg_struct_t sendOpt = nullptr;
    msg_get_struct_handle(req, MSG_REQUEST_SENDOPT_HND, &sendOpt);
    msg_set_bool_value(sendOpt, MSG_SEND_OPT_SETTING_BOOL, false);

    MessagePrivate &privMsg = dynamic_cast<MessagePrivate&>(msg);
    privMsg.commit();

    msg_set_struct_handle(req, MSG_REQUEST_MESSAGE_HND, privMsg);

    if (privMsg.getType() == Message::MT_SMS)
    {
        MSG_LOG("Sending SMS");
        err = msg_sms_send_message(m_ServiceHandle, req);
    }
    else if (privMsg.getType() == Message::MT_MMS)
    {
        MSG_LOG("Sending MMS");
        err = msg_mms_send_message(m_ServiceHandle, req);
    }
    else
    {
        msg_release_struct(&req);
        return SendFail;
    }

    if(threadId)
        msg_get_thread_id_by_address2(m_ServiceHandle, privMsg.getAddressList(), (msg_thread_id_t*)threadId);
    msg_release_struct(&req);

    if(err == MSG_SUCCESS)
    {
        MSG_LOG("sending success");
        return SendSuccess;
    }
    else if (err == MSG_ERR_INVALID_PARAMETER)
    {
        MSG_LOG_ERROR("sending failed error code INVALID_PARAM: ", err);
        return SendNullPointer;
    }
    else if (err == MSG_ERR_NO_SIM)
    {
        MSG_LOG_ERROR("sending failed error code NO SIM: ", err);
        return SendNoSIM;
    }
    else if (err == MSG_ERR_PLUGIN_STORAGE)
    {
        MSG_LOG_ERROR("sending failed error code MSG_ERR_PLUGIN_STORAGE: ", err);
        return SendMemoryFull;
    }
    else
    {
        MSG_LOG_ERROR("[DEBUG] sending failed error code: ", err);
        return SendFail;
    }
}

