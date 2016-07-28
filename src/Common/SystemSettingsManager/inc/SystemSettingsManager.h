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

#ifndef __SystemSettingsManager_h__
#define __SystemSettingsManager_h__

#include <vector>
#include <telephony.h>

namespace Msg
{
    class ISystemSettingsManager;

    /**
     * @brief Monitors changing of time format and system language. Also reads telephony settings.
     */
    class SystemSettingsManager
    {
        public:
            /**
             * @brief A constructor initializes telephony handle to make system settings manager instance to read telephony settings.
             */
            SystemSettingsManager();
            ~SystemSettingsManager();

            /**
             * @brief Adds a subscriber on system settings change events.
             * @param[in] l a listener.
             */
            void addListener(ISystemSettingsManager &l);

            /**
             * @brief Removes system settings changes listener.
             * @param[in] l a listener to be removed.
             */
            void removeListener(ISystemSettingsManager&l);

            /**
             * @brief Checks whether SIM is inserted or not.
             * @return true if SIM is inserted, false otherwise.
             */
            bool isSimInserted() const;

            /**
             * @brief Checks whether network service is available or not.
             * @return true network service available, false otherwise(service is unavailable or only emergency calls are available).
             */
            bool isSimActive() const;

            /**
             * @brief Checks whether mobile data is enabled or not.
             * @return true 3g mobile data is enabled, false otherwise.
             */
            bool isMobileDataEnabled() const;

        private:
            SystemSettingsManager(SystemSettingsManager&) = delete;
            SystemSettingsManager &operator =(const SystemSettingsManager&) = delete;

            void onTimeFormatChanged();
            void onLanguageChanged();

        private:
            std::vector<ISystemSettingsManager*> m_Listeners;
            telephony_handle_list_s m_TelHandleList;
    };

    /**
     * @brief A listener-interface that should be implemented by subscriber in order to be notified about time format or nsystem language change.
     */
    class ISystemSettingsManager
    {
        public:
            virtual ~ISystemSettingsManager() {}

            /**
             * @brief Notification about change of time format.
             */
            virtual void onTimeFormatChanged() {};

            /**
             * @brief Notification about change of system language.
             */
            virtual void onLanguageChanged() {};
    };
}

#endif /* __SystemSettingsManager_h__ */
