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

#ifndef __MsgSettingsPrivate_h__
#define __MsgSettingsPrivate_h__

#include "MsgSettings.h"
#include <msg_types.h>

namespace Msg
{
    class MsgSettingsPrivate
        : public MsgSettings
    {
        public:
            MsgSettingsPrivate(msg_handle_t serviceHandle);
            MsgSettingsPrivate(MsgSettingsPrivate&) = delete;
            MsgSettingsPrivate &operator=(MsgSettingsPrivate&) = delete;
            virtual ~MsgSettingsPrivate();

            virtual int getAddressMaxLen() const;
            virtual int getPhoneNumberMaxLen() const;
            virtual int getAddressMaxCount() const;
            virtual int getSubjectMaxLen() const;
            virtual int getDisplayNameMaxLen() const;
            virtual int getMaxRecipientCount() const;

            virtual void setAlerts(bool value);
            virtual bool getAlerts() const;

            virtual void setNotiSound(const std::string &soundPath);
            virtual std::string getNotiSound() const;

            virtual void setVibration(bool value);
            virtual bool getVibration() const;

            virtual void setSmsDelivReport(bool value);
            virtual bool getSmsDelivReport() const;

            virtual void setMmsDelivReport(bool value);
            virtual bool getMmsDelivReport() const ;

            virtual void setMmsReadReport(bool value);
            virtual bool getMmsReadReport() const;

            virtual void setMmsAutoRetr(bool value);
            virtual bool getMmsAutoRetr() const;

            virtual void setMmsAutoRetrRoaming(bool value);
            virtual bool getMmsAutoRetrRoaming() const;

        private:
            void init();
            void finit();

        private:
            msg_handle_t m_ServiceHandle;
            msg_struct_t m_GeneralOpt;
            msg_struct_t m_SmsSendOpt;
            msg_struct_t m_MmsSendOpt;
            msg_struct_t m_MmsRecvOpt;
    };
}

#endif /* __MsgSettingsPrivate_h__ */
