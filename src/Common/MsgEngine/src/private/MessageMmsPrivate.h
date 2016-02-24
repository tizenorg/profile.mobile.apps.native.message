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

#ifndef _MESSAGE_MMS_PRIVATE_H__
#define _MESSAGE_MMS_PRIVATE_H__

#include "MessageMms.h"
#include "MessagePrivate.h"
#include "MsgPagePrivate.h"
#include "MsgAttachmentPrivate.h"

namespace Msg
{
    class MessageMmsPrivate
        : public MessagePrivate
        , public MessageMms
    {
        public:
            MessageMmsPrivate(bool release, msg_struct_t msgStruct = nullptr, msg_struct_t mmsStruct = nullptr);
            virtual ~MessageMmsPrivate();

            virtual void setText(const std::string &text);
            virtual std::string getText() const;
            virtual Type getType() const;

            virtual const MsgPageListHandlePrivate &getPageList() const;
            virtual MsgPagePrivate &addPage();

            virtual const MsgAttachmentListHandlePrivate &getAttachmentList() const;
            virtual MsgAttachmentPrivate &addAttachment();

            void set(msg_struct_t msgStruct);
            msg_struct_t getMmsStruct() const;
            virtual void commit();

        private:
            msg_struct_t m_MmsStruct;
            mutable MsgPageListHandlePrivate m_PageList;
            mutable MsgAttachmentListHandlePrivate m_AttachmentList;
            MsgPagePrivate m_Page;
            MsgAttachmentPrivate m_Attachment;
    };

    typedef class MsgListHandlePrivate<MessageMmsPrivate, MessageMms> MmsListHandlePrivate;
    typedef class MsgStructListPrivate<MessageMmsPrivate, MessageMms> MmsStructListPrivate;
}

#endif /* _MESSAGE_MMS_PRIVATE_H__ */
