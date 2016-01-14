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
 *
 */

#ifndef AppControlNewMessage_h_
#define AppControlNewMessage_h_

#include "AppControlCommand.h"
#include "MsgTypes.h"
#include "Logger.h"
#include <string>

namespace Msg
{
    class AppControlNewMessage;
    typedef std::shared_ptr<AppControlNewMessage> AppControlNewMessageRef;

    class AppControlNewMessage
        : public AppControlCommand
    {
        public:
            enum NewMessageType
            {
                UnknownType,
                ReplyType, //Type for reply message button clicked
                NewMsgType, //Type for view message button clicked
                NotificationType //Type from notification panel
            };

        public:
            AppControlNewMessage(const std::string &opMsg, app_control_h handle);
            virtual ~AppControlNewMessage();

            MsgId getMessageId() const;
            NewMessageType getNewMessageType() const;

        private:
            bool isNotificationPanel(app_control_h handle) const;
            void parce(app_control_h handle);
            void replyMessage(app_control_h handle);
            void viewMessage(app_control_h handle);

        private:
            MsgId m_MsgId;
            NewMessageType m_NewMsgType;
    };
}

#endif /* AppControlNewMessage_h_ */
