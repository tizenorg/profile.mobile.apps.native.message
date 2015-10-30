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
#include "Logger.h"

namespace Msg
{
    class MsgEngine
    {
        public:
            MsgEngine();
            ~MsgEngine();

            int openService();
            int closeService();

            bool isReady(std::string &errorMsg) const;

            MsgStorage &getStorage();
            const MsgStorage &getStorage() const;

            MsgTransport &getTransport();
            const MsgTransport &getTransport() const;

            MsgSettings &getSettings();
            const MsgSettings &getSettings() const;

            static std::string whatError(int error);
            int calculateTextLen(const std::string &text) const;

        private:

        private:
            std::unique_ptr<MsgStorage>  m_MsgStorage;
            std::unique_ptr<MsgTransport> m_MsgTransport;
            std::unique_ptr<MsgSettings> m_MsgSettings;
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
