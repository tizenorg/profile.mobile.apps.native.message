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

#ifndef _MSG_MEDIA_PRIVATE_H__
#define _MSG_MEDIA_PRIVATE_H__

#include "MsgMedia.h"
#include "MsgStructPrivate.h"
#include "MsgListHandlePrivate.h"
#include "MsgStructListPrivate.h"

namespace Msg
{
    class MsgMediaPrivate
        : public MsgStructPrivate
        , public MsgMedia
    {
        public:
            MsgMediaPrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgMediaPrivate();

            virtual SmilType getType() const;
            virtual std::string getFilePath() const;
            virtual std::string getFileName() const;

            virtual void setType(SmilType type);
            virtual void setFileName(const std::string &name);
            virtual void setFilePath(const std::string &path);

            std::string getMime() const;
            void setMime(const std::string &mime);
    };

    typedef class MsgListHandlePrivate<MsgMediaPrivate, MsgMedia> MsgMediaListHandlePrivate;
    typedef class MsgStructListPrivate<MsgMediaPrivate, MsgMedia> MsgMediaStructListPrivate;
}

#endif /* _MSG_MEDIA_PRIVATE_H__ */
