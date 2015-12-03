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

#ifndef _MSG_PAGE_PRIVATE_H__
#define _MSG_PAGE_PRIVATE_H__

#include "MsgPage.h"
#include "MsgMediaPrivate.h"

namespace Msg
{
    class MsgPagePrivate
        : public MsgStructPrivate
        , public MsgPage
    {
        public:
            MsgPagePrivate(bool release, msg_struct_t msgStruct = nullptr);
            virtual ~MsgPagePrivate();

            virtual MsgMediaListHandlePrivate &getMediaList();
            virtual MsgMediaPrivate &addMedia();
            virtual void setPageDuration(int duration);
            virtual int getPageDuration() const;

        private:
            MsgMediaListHandlePrivate m_MediaList;
            MsgMediaPrivate m_Media;
    };

    typedef class MsgListHandlePrivate<MsgPagePrivate, MsgPage> MsgPageListHandlePrivate;
    typedef class MsgStructListPrivate<MsgPagePrivate, MsgPage> MsgPageStructListPrivate;
}

#endif /* _MSG_PAGE_PRIVATE_H__ */
