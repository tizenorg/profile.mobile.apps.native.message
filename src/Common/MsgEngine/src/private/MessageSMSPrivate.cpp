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
#include "Logger.h"
#include "MsgUtilsPrivate.h"

#include <string>
#include <msg.h>
#include <msg_storage.h>
#include <msg_transport.h>
#include <msg_types.h>


using namespace Msg;

MessageSMSPrivate::MessageSMSPrivate(bool release, msg_struct_t msgStruct)
    : MessagePrivate(release, msgStruct)
    , MessageSMS()
{

}

MessageSMSPrivate::~MessageSMSPrivate()
{
}

MessageSMSPrivate::Type MessageSMSPrivate::getType() const
{
    return MT_SMS;
}

void MessageSMSPrivate::setText(const std::string &text)
{
    msg_set_str_value(m_MsgStruct, MSG_MESSAGE_SMS_DATA_STR, text.c_str(), text.length());
}

std::string MessageSMSPrivate::getText() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MESSAGE_SMS_DATA_STR, MAX_MSG_DATA_LEN);
}

