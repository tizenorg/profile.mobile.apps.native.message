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
 *
 */

#ifndef __MsgSettings_h__
#define __MsgSettings_h__

#include <string>
#include <vector>

namespace Msg
{
    class IMsgSettingsListener;

    class MsgSettings
    {
        public:
            MsgSettings();
            virtual ~MsgSettings();

            void addListener(IMsgSettingsListener &listener);
            void removeListener(IMsgSettingsListener &listener);

            int getMessageTextMaxChar() const;
            int getMessageMaxPage() const;
            virtual int getAddressMaxLen() const = 0;
            virtual int getPhoneNumberMaxLen() const = 0;
            virtual int getAddressMaxCount() const = 0;
            virtual int getSubjectMaxLen() const = 0;
            virtual int getDisplayNameMaxLen() const = 0;

            virtual void setAlerts(bool value) = 0;
            virtual bool getAlerts() const = 0;

            virtual void setNotiSound(const std::string &soundPath) = 0;
            virtual std::string getNotiSound() const = 0;

            virtual void setVibration(bool value) = 0;
            virtual bool getVibration() const = 0;

            virtual void setSmsDelivReport(bool value) = 0;
            virtual bool getSmsDelivReport() const = 0;

            virtual void setMmsDelivReport(bool value) = 0;
            virtual bool getMmsDelivReport() const = 0;

            virtual void setMmsReadReport(bool value) = 0;
            virtual bool getMmsReadReport() const = 0;

            virtual void setMmsAutoRetr(bool value) = 0;
            virtual bool getMmsAutoRetr() const = 0;

            virtual void setMmsAutoRetrRoaming(bool value) = 0;
            virtual bool getMmsAutoRetrRoaming() const = 0;

        protected:
            typedef std::vector<IMsgSettingsListener *> MsgSettingsListeners;

        protected:
            MsgSettingsListeners m_Listeners;

        private:
            MsgSettings(MsgSettings&) = delete;
            MsgSettings& operator=(MsgSettings&) = delete;
    };

    class IMsgSettingsListener
    {
        public:
            virtual ~IMsgSettingsListener() {}
            virtual void onNotiSoundChanged(MsgSettings &msgSetting) {};
    };
}


#endif /* __MsgSettings_h__ */
