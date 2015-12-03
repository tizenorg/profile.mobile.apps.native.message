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

#ifndef _MSG_PAGE_H__
#define _MSG_PAGE_H__

#include "MsgMedia.h"

namespace Msg
{
    class MsgPage;
    typedef std::shared_ptr<MsgPage> MsgPageRef;
    typedef MsgList<MsgPage> MsgPageList;
    typedef std::shared_ptr<MsgList<MsgPage>> MsgPageListRef;

    class MsgPage
    {
        public:
            virtual ~MsgPage();

            virtual MsgMediaList &getMediaList() = 0;
            const MsgMediaList &getMediaList() const;
            virtual MsgMedia &addMedia() = 0;
            virtual void setPageDuration(int duration) = 0;
            virtual int getPageDuration() const = 0;
    };
}

#endif /* _MSG_PAGE_H__ */
