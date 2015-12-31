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

namespace Msg
{
    class AppControlCompose
        : public AppControlCommand
    {
        public:
            AppControlCompose(const std::string &opMsg, app_control_h handle);
            virtual ~AppControlCompose();

            virtual const RecipientList &getRecipientList() const;
            virtual MessageType getMessageType() const;
            virtual const std::string getMessageText() const;
            virtual const std::string getMessageSubject() const;
            virtual const FileList &getFileList() const;

        private:
            bool parseUri(const char *uri);
    };
}

#endif /* AppControlCompose_h_ */
