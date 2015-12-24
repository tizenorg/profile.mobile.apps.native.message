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

#ifndef AppControlCompose_h_
#define AppControlCompose_h_

#include "AppControlCommand.h"
#include <app_control.h>
#include <string>
#include <list>

namespace Msg
{
    class AppControlCompose
        : public AppControlCommand
    {
        public:
            typedef std::list<std::string> RecipientList;
            typedef std::list<std::string> FileList;

            enum MessageType
            {
                UnknownType,
                MmsType,
                SmsType
            };
        public:
            AppControlCompose(const std::string &opMsg);
            virtual ~AppControlCompose();

            virtual bool initialize(app_control_h handle);

            const RecipientList &getRecipientList() const;
            MessageType getMessageType() const;
            const std::string getMessageText() const;
            const std::string getMessageSubject() const;
            const FileList &getFileList() const;

        private:
            bool parseUri(const char* uri);

        private:
            RecipientList m_RecipientList;
            MessageType m_MessageType;
            FileList m_FileList;
            std::string m_messageText;
            std::string m_subject;
    };
}

#endif /* AppControlCompose_h_ */
