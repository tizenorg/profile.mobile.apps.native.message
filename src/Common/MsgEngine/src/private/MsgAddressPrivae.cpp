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

#include "MsgAddressPrivate.h"
#include "MsgUtilsPrivate.h"

#include <msg.h>

using namespace Msg;

MsgAddressPrivate::MsgAddressPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , MsgAddress()
{

}

MsgAddressPrivate::~MsgAddressPrivate()
{

}

std::string MsgAddressPrivate::getAddress() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, MAX_ADDRESS_VAL_LEN);
}

MsgAddressPrivate::AddressType MsgAddressPrivate::getAddressType() const
{
    int nativeType = MSG_ADDRESS_TYPE_UNKNOWN;
    msg_get_int_value(m_MsgStruct, MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, &nativeType);
    return MsgUtilsPrivate::nativeToAddressType(nativeType);
}

MsgAddressPrivate::RecipientType MsgAddressPrivate::getRecipientType() const
{
    int nativeType = MSG_RECIPIENTS_TYPE_UNKNOWN;
    msg_get_int_value(m_MsgStruct, MSG_ADDRESS_INFO_RECIPIENT_TYPE_INT, &nativeType);
    return MsgUtilsPrivate::nativeToRecipientType(nativeType);
}

void MsgAddressPrivate::setAddress(const std::string &address)
{
    msg_set_str_value(m_MsgStruct, MSG_ADDRESS_INFO_ADDRESS_VALUE_STR, const_cast<char *>(address.c_str()), address.length());
}

void MsgAddressPrivate::setAddressType(AddressType type)
{
    msg_set_int_value(m_MsgStruct, MSG_ADDRESS_INFO_ADDRESS_TYPE_INT, MsgUtilsPrivate::addressTypeToNative(type));
}

void MsgAddressPrivate::setRecipientType(RecipientType type)
{
    msg_set_int_value(m_MsgStruct, MSG_ADDRESS_INFO_RECIPIENT_TYPE_INT, MsgUtilsPrivate::recipientTypeToNative(type));
}

void MsgAddressPrivate::setFields(const MsgAddress &addr)
{
    setAddress(addr.getAddress());
    setAddressType(addr.getAddressType());
    setRecipientType(addr.getRecipientType());
}
