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

#ifndef _MSG_MEDIA_H__
#define _MSG_MEDIA_H__

#include "MsgList.h"
#include <memory>

namespace Msg
{
    class MsgMedia;
    typedef std::shared_ptr<MsgMedia> MsgMediaRef;
    typedef MsgList<MsgMedia> MsgMediaList;
    typedef std::shared_ptr<MsgList<MsgMedia>> MsgMediaListRef;

    class MsgMedia
    {
        public:
            enum Type
            {
                UnknownType = 0,
                ImageType,
                AudioType,
                VideoType,
                TextType,
                AnimateType,
                ImageOrVideoType
            };

        public:
            virtual ~MsgMedia() {};

            virtual Type getType() const = 0;
            virtual std::string getFilePath() const = 0;
            virtual std::string getFileName() const = 0;

            virtual void setFilePath(const std::string &path) = 0;
            virtual void setFileName(const std::string &name) = 0;
    };
}

#endif /* _MSG_MEDIA_H__ */
