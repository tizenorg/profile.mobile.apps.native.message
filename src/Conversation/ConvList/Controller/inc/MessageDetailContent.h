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
#ifndef MessageDetailContent_H_
#define MessageDetailContent_H_

#include "MsgTypes.h"
#include "Message.h"
#include "App.h"
#include <string>

namespace Msg
{
    class MessageDetailContent
    {
        public:
            static std::string getMsgDetailContent(App &app, MsgId msgId);
            static std::string getMmsNotiDetailContent(App &app, MsgId msgId);

        private:
            // Create Content of Popup for View Details
            static std::string createMsgDetailsText(App &app, MsgId msgId);
            static std::string getMessageType(Message::Type msgType);
            static std::string getContactsInfo(App &app, Message::Direction m_Direction, ThreadId msgThreadId);
            static std::string getSentReceivedTime(MsgStorage &msgStorage, Message::Direction msgDirection, MsgId msgId);
            static std::string makeReportResult(App &app, Message::NetworkStatus msgStatus, Message::Type msgType, ThreadId msgThreadId, MsgId msgId);
            static std::string getSubscriberNumber();
            static std::string getSmsStatus(Message::NetworkStatus msgStatus);
            static std::string getMmsSubject(App &app, MsgId msgId);
            static std::string getMmsMessageSize(App &app, MsgId msgId);
            static std::string getMmsMessageExpired(App &app, MsgId msgId);
            static std::string makeReadReportResult(App &app, MsgId msgId, ThreadId msgThreadId, Message::NetworkStatus msgStatus);
    };
}

#endif /* MessageDetailContent_H_ */
