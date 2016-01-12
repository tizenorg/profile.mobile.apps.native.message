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

#ifndef ThreadSearchListItem_h_
#define ThreadSearchListItem_h_

#include "BaseThreadListItem.h"
#include "MsgThreadItem.h"
#include "MsgTypes.h"

namespace Msg
{
    class ThreadSearchListItem
        : public BaseThreadListItem
    {
        public:
            ThreadSearchListItem(App &app, const MsgThreadItem &item, const std::string &searchWord);
            virtual ~ThreadSearchListItem();

            ThreadId getThreadId() const;

        private:
            void update(const MsgThreadItem &item, const std::string &searchWord);

        private:
            ThreadId m_ThreadId;
    };
}

#endif // ThreadSearchListItem_h_
