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

#ifndef __MSG_COMPOSER_PRIVATE_H__
#define __MSG_COMPOSER_PRIVATE_H__

#include "MsgComposer.h"
#include <msg_types.h>

namespace Msg
{
    class MsgComposerPrivate
        : public MsgComposer
    {
        public:
            MsgComposerPrivate(msg_handle_t serviceHandle);
            MsgComposerPrivate(MsgComposerPrivate&) = delete;
            MsgComposerPrivate &operator=(MsgComposerPrivate&) = delete;
            virtual ~MsgComposerPrivate();

            virtual MessageSMSRef createSms();
            virtual MessageMmsRef createMms();

        private:
            void setSmilHeader(msg_struct_t mms, bool isTextTop);

        private:
            msg_handle_t m_ServiceHandle;
    };
}

#endif /* __MSG_COMPOSER_PRIVATE_H__ */
