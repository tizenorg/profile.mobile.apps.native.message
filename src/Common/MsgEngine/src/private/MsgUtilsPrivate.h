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

#include <msg_types.h>
#include <assert.h>

namespace Msg
{
    class MsgUtilsPrivate
    {
        public:
            inline static int directionToNative(Message::Direction direction);
            inline static int addressTypeToNative(MsgAddress::AddressType type);
            inline static int recipientTypeToNative(MsgAddress::RecipientType type);

            inline static Message::Direction nativeToDirection(int direction);
            inline static MsgAddress::AddressType nativeToAddressType(int type);
            inline static MsgAddress::RecipientType nativeToRecipientType(int type);
    };

    inline int MsgUtilsPrivate::directionToNative(Message::Direction direction)
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

    inline Message::Direction MsgUtilsPrivate::nativeToDirection(int direction)
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

    inline int MsgUtilsPrivate::addressTypeToNative(MsgAddress::AddressType type)
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

      inline MsgAddress::AddressType MsgUtilsPrivate::nativeToAddressType(int type)
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

      inline int MsgUtilsPrivate::recipientTypeToNative(MsgAddress::RecipientType type)
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

      inline MsgAddress::RecipientType MsgUtilsPrivate::nativeToRecipientType(int type)
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
}

#endif /* _MESSAGE_UTILS_PRIVATE_H__ */
