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

using namespace Msg;

MsgTransportPrivate::MsgTransportPrivate(msg_handle_t serviceHandle)
    : MsgTransport()
    , m_ServiceHandleImpl(serviceHandle)
{

}

MsgTransportPrivate::~MsgTransportPrivate()
{

}

void MsgTransportPrivate::sendMessage(const MessageRef msg)
{
    msg_struct_t req = msg_create_struct(MSG_STRUCT_REQUEST_INFO);

    msg_struct_t sendOpt = nullptr;
    msg_get_struct_handle(req, MSG_REQUEST_SENDOPT_HND, &sendOpt);
    msg_set_bool_value(sendOpt, MSG_SEND_OPT_SETTING_BOOL, false);

    MessagePrivate &privMsg = dynamic_cast<MessagePrivate&>(*msg);

    msg_set_struct_handle(req, MSG_REQUEST_MESSAGE_HND, privMsg.getMsgStruct());
    msg_sms_send_message(m_ServiceHandleImpl, req);

    msg_release_struct(&req);
}
