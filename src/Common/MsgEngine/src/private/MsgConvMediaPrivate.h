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

#ifndef MSGCONVMEDIAPRIVATE_H_
#define MSGCONVMEDIAPRIVATE_H_

#include "MsgConvMedia.h"
#include "MsgListHandlePrivate.h"
#include "MsgStructPrivate.h"

namespace Msg
{
    class MsgConvMediaPrivate
        : public MsgStructPrivate
        , public MsgConvMedia
    {
        public:
            MsgConvMediaPrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgConvMediaPrivate();

            virtual std::string getMime() const;
            virtual std::string getName() const;
            virtual std::string getPath() const;
            virtual std::string getThumbPath() const;
            virtual std::string getContentId() const;
            virtual std::string getContentLocation() const;

    };

    typedef class MsgListHandlePrivate<MsgConvMediaPrivate, MsgConvMedia> MsgConvMediaListHandlePrivate;
}

#endif /* MSGCONVMEDIAPRIVATE_H_ */
