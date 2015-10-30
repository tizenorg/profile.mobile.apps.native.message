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

#ifndef _MSG_DATA_CONTAINER_H__
#define _MSG_DATA_CONTAINER_H__

#include <string>
#include "Message.h"

namespace Msg
{
    class MsgDataContainer
    {
        public:
            MsgDataContainer();
            ~MsgDataContainer();

            void setText(const std::string &text);
            const std::string &getText() const;
            const AddressList &getAddressList() const;

            void  setType(Message::Type type);
            Message::Type getType() const;

            void addAddressList(const AddressList &newList);
            void addAddress(const std::string &address);
            void removeAddress(const std::string &address);

        private:
            Message::Type m_Type;
            std::string m_Text;
            AddressList m_AddressList;
    };
}


#endif /* _MSG_DATA_CONTAINER_H__ */
