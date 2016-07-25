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

namespace
{
    const int _MSG_ERR_DPM_RESTRICT = -200; //Temporary define. Delete this when msg service with DPM will be available in rootstrap.
};

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

    if(privMsg.isMms())
    {
        MSG_LOG("Sending MMS");
        err = msg_mms_send_message(m_ServiceHandle, req);
    }
    else
    {
        MSG_LOG("Sending SMS");
        err = msg_sms_send_message(m_ServiceHandle, req);
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
    else if (err == _MSG_ERR_DPM_RESTRICT)
    {
        MSG_LOG_ERROR("sending failed error code MSG_ERR_DPM_RESTRICT: ", err);
        return SendDPMRestricted;
    }
    else
    {
        MSG_LOG_ERROR("[DEBUG] sending failed error code: ", err);
        return SendFail;
    }
}

void MsgTransportPrivate::retrieveMessage(MsgId msgId)
{
    msg_struct_t req = msg_create_struct(MSG_STRUCT_REQUEST_INFO);
    msg_struct_t sendOpt = msg_create_struct(MSG_STRUCT_SENDOPT);
    msg_struct_t retrieveMsg = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);

    msg_get_message(m_ServiceHandle, msgId, retrieveMsg, sendOpt);
    msg_set_struct_handle(req, MSG_REQUEST_MESSAGE_HND, retrieveMsg);
    msg_set_struct_handle(req, MSG_REQUEST_SENDOPT_HND, sendOpt);

    msg_error_t err = msg_mms_retrieve_message(m_ServiceHandle, req);
    MSG_LOG("Retrieve message result: ", err);

    msg_release_struct(&retrieveMsg);
    msg_release_struct(&sendOpt);
    msg_release_struct(&req);
}
