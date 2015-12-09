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

#ifndef MSGTYPES_H_
#define MSGTYPES_H_

#include <vector>

namespace Msg
{
    class BaseMsgId
    {
        public:
            BaseMsgId()
                : value(-1)
            {
            }

            BaseMsgId(int id)
                : value(id)
            {
            }

            BaseMsgId(const BaseMsgId &obj)
                : value(obj.value)
            {
            }

            operator int() const
            {
                return value;
            }

            BaseMsgId &operator=(int id)
            {
                value = id;
                return *this;
            }

            bool isValid() const
            {
                return value > 0; // TODO: check
            }

            int value;
    };

    class ThreadId
        : public BaseMsgId
    {
        using BaseMsgId::BaseMsgId;
    };

    class MsgId
        : public BaseMsgId
    {
        using BaseMsgId::BaseMsgId;
    };

    typedef std::vector<MsgId> MsgIdList;
}

#endif /* MSGTYPES_H_ */
