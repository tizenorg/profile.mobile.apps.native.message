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

#ifndef __MSG_ENGINE_H__
#define __MSG_ENGINE_H__

#include <string>
#include <memory>

#ifdef TIZEN_PRIVATE_API
#include <msg_types.h>
#else
#include <messages_types.h>
#endif

#include "MsgStorage.h"
#include "MsgTransport.h"
#include "MsgSettings.h"
#include "MsgComposer.h"
#include "Logger.h"
#include "MsgTypes.h"

namespace Msg
{
    /**
     * @brief Core-instance of whole mechanism that interacts with message-service.
     */
    class MsgEngine
    {
        public:
            MsgEngine();
            ~MsgEngine();

            /**
             * @brief Initiates connection to message-service.
             * @returns msg-service specific success code in case of success, otherwise returns error-code.
             */
            int openService();

            /**
             * @brief Closes connection to message-service.
             * @returns msg-service specific success code in case of success, otherwise returns error-code.
             */
            int closeService();

            /**
             * @brief Checks whether message-service opened is ready to operate(was openService() called before or not).
             * @param[out] error-message string filled if no connection to message service was opened.
             * @result true if message-service is already opened, otherwise false.
             */
            bool isReady(std::string &errorMsg) const;

            MsgStorage &getStorage();
            const MsgStorage &getStorage() const;

            MsgTransport &getTransport();
            const MsgTransport &getTransport() const;

            MsgSettings &getSettings();
            const MsgSettings &getSettings() const;

            MsgComposer &getComposer();
            const MsgComposer &getComposer() const;

            /**
             * @brief converts msg-service error codes into a string representation.
             * @param[in] error an msg-service side error code
             * @result msg-service define name string, e.g "MESSAGES_ERROR_NONE" for MESSAGES_ERROR_NONE code.
             */
            static std::string whatError(int error);

            /**
             * @brief Calculates input text length by encode type.
             * @param[in] text input text.
             * @param[out] text metric.
             **/
            static void calculateTextMetric(const std::string &text, MsgTextMetric &textMetric);

        private:
            std::unique_ptr<MsgStorage>  m_Storage;
            std::unique_ptr<MsgTransport> m_Transport;
            std::unique_ptr<MsgSettings> m_Settings;
            std::unique_ptr<MsgComposer> m_Composer;
        #ifdef TIZEN_PRIVATE_API
            msg_handle_t m_MsgHandle;
        #else
            messages_service_h m_MsgHandle;
        #endif

    };
}

#define MSG_ENGINE_WHAT_ERROR(error)\
    MSG_LOG_INFO("Error = ", MsgEngine::whatError(error))


#endif /* __MSG_ENGINE_H__ */
