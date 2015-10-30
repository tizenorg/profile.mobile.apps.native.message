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

#include "MessageSMSPrivate.h"
#include <string>
#include <msg.h>
#include <msg_storage.h>
#include <msg_transport.h>
#include "Logger.h"

using namespace Msg;

const std::string someBodyText = "Some message to send";
const std::string somePhoneAddress = "+380978091988";

MessageSMSPrivate::MessageSMSPrivate(msg_handle_t mainHandle)
    : MessagePrivate(mainHandle)
{
    initBaseHandle();
}

MessageSMSPrivate::MessageSMSPrivate(msg_handle_t mainHandle, msg_struct_t msgStruct)
	: MessagePrivate(mainHandle)
{
	setMsgStruct(msgStruct);
}

MessageSMSPrivate::~MessageSMSPrivate()
{
}

MessageSMSPrivate::Type MessageSMSPrivate::getType() const
{
    return MT_SMS;
}

void MessageSMSPrivate::initBaseHandle()
{
    msg_struct_t msgInfo = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);

    //msg_set_int_value(msgInfo, MSG_MESSAGE_ID_INT, msg_id);//
    msg_set_int_value(msgInfo, MSG_MESSAGE_TYPE_INT, MSG_TYPE_SMS);
    msg_set_int_value(msgInfo, MSG_MESSAGE_ENCODE_TYPE_INT, MSG_ENCODE_AUTO);//

    setMsgStruct(msgInfo);
}

void MessageSMSPrivate::setText(const std::string &text)
{
    msg_set_str_value(getMsgStruct(), MSG_MESSAGE_SMS_DATA_STR, const_cast<char *>(text.c_str()), text.length());
}

std::string MessageSMSPrivate::getText() const
{
    int msgSize = 0;
    msg_get_int_value(getMsgStruct(), MSG_MESSAGE_DATA_SIZE_INT, &msgSize);
    MSG_LOG("MsgStruct ", getMsgStruct());
    MSG_LOG("msgSize", msgSize);
    char msgText[msgSize] = {0};

    msg_get_str_value(getMsgStruct(), MSG_MESSAGE_SMS_DATA_STR, msgText, msgSize);
    std::string res(msgText);
    return res;
}
