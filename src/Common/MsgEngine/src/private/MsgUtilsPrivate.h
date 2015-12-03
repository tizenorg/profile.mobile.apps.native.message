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

#include <msg_types.h>
#include <msg.h>
#include <assert.h>
#include <type_traits>

namespace Msg
{
    class MsgUtilsPrivate
    {
        public:
            inline static int directionToNative(Message::Direction direction);
            inline static int addressTypeToNative(MsgAddress::AddressType type);
            inline static int recipientTypeToNative(MsgAddress::RecipientType type);
            inline static int messageTypeToNative(Message::Type type);
            inline static Message::Direction nativeToDirection(int direction);
            inline static MsgAddress::AddressType nativeToAddressType(int type);
            inline static MsgAddress::RecipientType nativeToRecipientType(int type);
            inline static Message::Type nativeToMessageType(int type);
            inline static MsgMedia::SmilType nativeToSmilType(int type);
            inline static int smilTypeToNative(MsgMedia::SmilType type);

            inline static std::string getStr(msg_struct_t msgStruct, int field, int maxStrLen);
            inline static int setStr(msg_struct_t msgStruct, int field, const std::string &text);

        private:
            inline static void checkSmilType();
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

      inline int MsgUtilsPrivate::messageTypeToNative(Message::Type type)
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

      inline Message::Type MsgUtilsPrivate::nativeToMessageType(int type)
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

      inline void MsgUtilsPrivate::checkSmilType()
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

      inline MsgMedia::SmilType MsgUtilsPrivate::nativeToSmilType(int type)
      {
          checkSmilType();
          return (MsgMedia::SmilType)type;
      }

      inline int MsgUtilsPrivate::smilTypeToNative(MsgMedia::SmilType type)
      {
          checkSmilType();
          return (int)type;
      }

      inline std::string MsgUtilsPrivate::getStr(msg_struct_t msgStruct, int field, int maxStrLen)
      {
          std::string res;
          char buf[maxStrLen + 1];
          if(msg_get_str_value(msgStruct, field, buf, maxStrLen) == 0)
          {
              res.assign(buf);
          }
          return res;
      }

      inline int MsgUtilsPrivate::setStr(msg_struct_t msgStruct, int field, const std::string &text)
      {
          return msg_set_str_value(msgStruct, field, (char*)text.c_str(), text.length());
      }
}

#endif /* _MESSAGE_UTILS_PRIVATE_H__ */
