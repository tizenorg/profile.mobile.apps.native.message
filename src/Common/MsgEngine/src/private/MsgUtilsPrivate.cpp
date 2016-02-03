/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "MsgUtilsPrivate.h"

using namespace Msg;


int MsgUtilsPrivate::directionToNative(Message::Direction direction)
{
    switch(direction)
    {
        case Message::MD_Sent:
            return MSG_DIRECTION_TYPE_MO;

        case Message::MD_Received:
            return MSG_DIRECTION_TYPE_MT;
    }
    assert(false);
    return MSG_DIRECTION_TYPE_MO;
}

Message::Direction MsgUtilsPrivate::nativeToDirection(int direction)
{
    switch(direction)
    {
        case MSG_DIRECTION_TYPE_MO:
            return Message::MD_Sent;

        case MSG_DIRECTION_TYPE_MT:
            return Message::MD_Received;
    }
    assert(false);
    return Message::MD_Sent;
}

int MsgUtilsPrivate::addressTypeToNative(MsgAddress::AddressType type)
{
    switch(type)
    {
        case MsgAddress::Phone:
            return MSG_ADDRESS_TYPE_PLMN;

        case MsgAddress::Email:
            return MSG_ADDRESS_TYPE_EMAIL;

        default:
        case MsgAddress::UnknownAddressType:
            return MSG_ADDRESS_TYPE_UNKNOWN;
    }
}

MsgAddress::AddressType MsgUtilsPrivate::nativeToAddressType(int type)
{
    switch(type)
    {
        case MSG_ADDRESS_TYPE_PLMN:
            return MsgAddress::Phone;

        case MSG_ADDRESS_TYPE_EMAIL:
            return MsgAddress::Email;

        default:
        case MSG_ADDRESS_TYPE_UNKNOWN:
            return MsgAddress::UnknownAddressType;
    }
}

int MsgUtilsPrivate::recipientTypeToNative(MsgAddress::RecipientType type)
{
    switch(type)
    {
        case MsgAddress::To:
            return MSG_RECIPIENTS_TYPE_TO;

        case MsgAddress::Cc:
            return MSG_RECIPIENTS_TYPE_CC;

        case MsgAddress::Bcc:
            return MSG_RECIPIENTS_TYPE_BCC;

        default:
        case MsgAddress::UnknownRecipientType:
            return MSG_RECIPIENTS_TYPE_UNKNOWN;
    }
}

MsgAddress::RecipientType MsgUtilsPrivate::nativeToRecipientType(int type)
{
    switch(type)
    {
        case MSG_RECIPIENTS_TYPE_TO:
            return MsgAddress::To;

        case MSG_RECIPIENTS_TYPE_CC:
            return MsgAddress::Cc;

        case MSG_RECIPIENTS_TYPE_BCC:
            return MsgAddress::Bcc;

        default:
        case MSG_RECIPIENTS_TYPE_UNKNOWN:
            return MsgAddress::UnknownRecipientType;
    }
}

int MsgUtilsPrivate::messageTypeToNative(Message::Type type)
{
    switch(type)
    {
        case Message::MT_SMS:
            return MSG_TYPE_SMS;

        case Message::MT_MMS:
            return MSG_TYPE_MMS;

        default:
            return MSG_TYPE_INVALID;
    }
}

Message::Type MsgUtilsPrivate::nativeToMessageType(int type)
{
    switch(type)
    {
        case MSG_TYPE_SMS:
            return Message::MT_SMS;

        case MSG_TYPE_MMS:
            return Message::MT_MMS;

        default:
            return Message::MT_Unknown;
    }
}

void MsgUtilsPrivate::checkSmilType()
{
    static_assert((int)MMS_SMIL_MEDIA_INVALID == (int)MsgMedia::SmilInvalid &&
        (int)MMS_SMIL_MEDIA_IMG == (int)MsgMedia::SmilImage &&
        (int)MMS_SMIL_MEDIA_AUDIO == (int)MsgMedia::SmilAudio &&
        (int)MMS_SMIL_MEDIA_VIDEO == (int)MsgMedia::SmilVideo &&
        (int)MMS_SMIL_MEDIA_TEXT == (int)MsgMedia::SmilText &&
        (int)MMS_SMIL_MEDIA_ANIMATE == (int)MsgMedia::SmilAnimate &&
        (int)MMS_SMIL_MEDIA_IMG_OR_VIDEO == (int)MsgMedia::SmilImageOrVideo &&
        (int)MMS_SMIL_MEDIA_MAX ==(int)MsgMedia::SmilMAX, "");
}

MsgMedia::SmilType MsgUtilsPrivate::nativeToSmilType(int type)
{
    checkSmilType();
    return (MsgMedia::SmilType)type;
}

int MsgUtilsPrivate::smilTypeToNative(MsgMedia::SmilType type)
{
    checkSmilType();
    return (int)type;
}

std::string MsgUtilsPrivate::getStr(msg_struct_t msgStruct, int field, int maxStrLen)
{
    std::string res;
    char buf[maxStrLen + 1];
    if(msg_get_str_value(msgStruct, field, buf, maxStrLen) == 0)
    {
        res.assign(buf);
    }
    return res;
}

int MsgUtilsPrivate::setStr(msg_struct_t msgStruct, int field, const std::string &text)
{
    return msg_set_str_value(msgStruct, field, text.c_str(), text.length());
}

Message::NetworkStatus MsgUtilsPrivate::nativeToNetworkStatus(int status)
{
    switch(status)
    {
        case MSG_NETWORK_NOT_SEND:
            return Message::NS_Not_Send;
        case MSG_NETWORK_SENDING:
            return Message::NS_Sending;
        case MSG_NETWORK_SEND_SUCCESS:
            return Message::NS_Send_Success;
        case MSG_NETWORK_SEND_FAIL:
        case MSG_NETWORK_SEND_TIMEOUT:
        case MSG_NETWORK_SEND_FAIL_MANDATORY_INFO_MISSING:
        case MSG_NETWORK_SEND_FAIL_TEMPORARY:
        case MSG_NETWORK_SEND_FAIL_BY_MO_CONTROL_WITH_MOD:
        case MSG_NETWORK_SEND_FAIL_BY_MO_CONTROL_NOT_ALLOWED:
            return Message::NS_Send_Fail;
        case MSG_NETWORK_DELIVER_SUCCESS:
            return Message::NS_Deliver_Success;
        case MSG_NETWORK_DELIVER_FAIL:
            return Message::NS_Deliver_Fail;
        case MSG_NETWORK_RECEIVED:
            return Message::NS_Received;
        case MSG_NETWORK_REQ_CANCELLED:
            return Message::NS_Req_Cancelled;
        case MSG_NETWORK_RETRIEVING:
            return Message::NS_Retrieving;
        case MSG_NETWORK_RETRIEVE_SUCCESS:
            return Message::NS_Retrieve_Success;
        case MSG_NETWORK_RETRIEVE_FAIL:
            return Message::NS_Retrieve_Fail;
        case MSG_NETWORK_DELIVER_PENDING:
            return Message::NS_Deliver_Pending;
        case MSG_NETWORK_DELIVER_EXPIRED:
            return Message::NS_Deliver_Expired;
        case MSG_NETWORK_SEND_PENDING:
            return Message::NS_Send_Pending;
        default:
            break;
    };
    return Message::NS_Unknown;
}
