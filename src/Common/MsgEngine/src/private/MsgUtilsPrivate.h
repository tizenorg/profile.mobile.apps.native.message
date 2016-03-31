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

#ifndef _MESSAGE_UTILS_PRIVATE_H__
#define _MESSAGE_UTILS_PRIVATE_H__

#include "Message.h"
#include "MsgMedia.h"
#include "MsgReport.h"

#include <msg_types.h>
#include <msg.h>
#include <assert.h>
#include <type_traits>

namespace Msg
{
    class MsgUtilsPrivate
    {
        public:
            static int directionToNative(Message::Direction direction);
            static int addressTypeToNative(MsgAddress::AddressType type);
            static int recipientTypeToNative(MsgAddress::RecipientType type);
            static Message::Direction nativeToDirection(int direction);
            static MsgAddress::AddressType nativeToAddressType(int type);
            static MsgAddress::RecipientType nativeToRecipientType(int type);
            static MsgReport::DeliveryStatus nativeToReportDeliveryStatus(int status);
            static MsgReport::ReadStatus nativeToReportReadStatus(int status);
            static MsgReport::Type nativeToReportType(int type);
            static Message::Type nativeToMessageType(int type);
            static MsgMedia::Type nativeToSmilType(int type);
            static int smilTypeToNative(MsgMedia::Type type);
            static Message::NetworkStatus nativeToNetworkStatus(int status);

            static std::string getStr(msg_struct_t msgStruct, int field, int maxStrLen);
            static int setStr(msg_struct_t msgStruct, int field, const std::string &text);

        private:
            static void checkSmilType();
    };
}

#endif /* _MESSAGE_UTILS_PRIVATE_H__ */
