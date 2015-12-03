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

#ifndef _MSG_ATTACHMENT_PRIVATE_H__
#define _MSG_ATTACHMENT_PRIVATE_H__

#include "MsgAttachment.h"
#include "MsgStructPrivate.h"
#include "MsgListHandlePrivate.h"
#include "MsgStructListPrivate.h"

namespace Msg
{
    class MsgAttachmentPrivate
        : public MsgStructPrivate
        , public MsgAttachment
    {
        public:
            MsgAttachmentPrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgAttachmentPrivate();

            virtual std::string getFilePath() const;
            virtual std::string getFileName() const;
            virtual int getFileSize() const;
            virtual std::string getMime() const;

            virtual void setFilePath(const std::string &path);
            virtual void setFileName(const std::string &fileName);
            virtual void setFileSize(int size) const;
            virtual void setMime(const std::string &mime);
    };

    typedef class MsgListHandlePrivate<MsgAttachmentPrivate, MsgAttachment> MsgAttachmentListHandlePrivate;
    typedef class MsgStructListPrivate<MsgAttachmentPrivate, MsgAttachment> MsgAttachmentStructListPrivate;
}

#endif /* _MSG_ATTACHMENT_PRIVATE_H__ */
