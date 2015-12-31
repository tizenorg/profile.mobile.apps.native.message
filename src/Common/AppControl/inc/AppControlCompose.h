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
    class AppControlCompose;
    typedef std::shared_ptr<AppControlCompose> AppControlComposeRef;

    class AppControlCompose
        : public AppControlCommand
    {
        public:
            typedef std::list<std::string> RecipientList;
            typedef std::list<std::string> FileList;

            enum OpComposeType
            {
                OpUnknown,
                OpCompose, /** APP_CONTROL_OPERATION_COMPOSE */
                OpShare, /** APP_CONTROL_OPERATION_SHARE */
                OpMultiShare, /** APP_CONTROL_OPERATION_MULTI_SHARE */
                OpShareText /** APP_CONTROL_OPERATION_SHARE_TEXT */
            };

        public:
            AppControlCompose(const std::string &opMsg, app_control_h handle);
            virtual ~AppControlCompose();

            /**
             * Gets compose type.
             */
            OpComposeType getComposeType() const;

            /**
             * Gets data from APP_CONTROL_DATA_TO or URI(if exists).
             */
            const RecipientList &getRecipientList() const;

            /**
             * Checks if is MMS.
             */
            bool isMms() const;

            /**
             * Gets data from APP_CONTROL_DATA_TEXT.
             */
            const std::string &getMessageText() const;

            /**
             * Gets data from APP_CONTROL_DATA_SUBJECT.
             */
            const std::string &getMessageSubject() const;

            /**
             * Gets data from APP_CONTROL_DATA_PATH or URI(if exists).
             */
            const FileList &getFileList() const;

        private:
            bool parseUri(const char *uri);

        private:
            OpComposeType m_ComposeType;
            RecipientList m_RecipientList;
            bool m_isMms;
            FileList m_FileList;
            std::string m_MessageText;
            std::string m_Subject;
    };


}

#endif /* AppControlCompose_h_ */
