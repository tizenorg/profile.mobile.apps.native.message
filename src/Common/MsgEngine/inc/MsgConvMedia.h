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

#ifndef MSGCONVMEDIA_H_
#define MSGCONVMEDIA_H_

#include "MsgList.h"

#include <memory>
#include <string>

namespace Msg
{
    class MsgConvMedia;
    typedef MsgList<MsgConvMedia> MsgConvMediaList;
    typedef std::shared_ptr<MsgList<MsgConvMedia>> MsgConvMediaListRef;

    class MsgConvMedia
    {
        public:
            virtual ~MsgConvMedia() {};

            virtual std::string getMime() const = 0;
            virtual std::string getName() const = 0;
            virtual std::string getPath() const = 0;
            virtual std::string getThumbPath() const = 0;
            virtual std::string getContentId() const = 0;
            virtual std::string getContentLocation() const = 0;
    };
}

#endif /* MSGCONVMEDIA_H_ */
