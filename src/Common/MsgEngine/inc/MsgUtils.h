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

#ifndef MsgUtils_H_
#define MsgUtils_H_

#include "MsgTypes.h"
#include "MsgAddress.h"
#include "Message.h"

#include <string>
#include <list>
#include <vector>

namespace Msg
{
    struct TokenizedRecipients
    {
        std::list<std::pair<std::string, MsgAddress::AddressType>> validResults;
        std::string invalidResult;
    };

    class MsgUtils
    {
        public:
            static TokenizedRecipients tokenizeRecipients(const std::string &inputText);
            static bool isValidNumber(const std::string &address);
            static bool isValidEmail(const std::string &address);
            static MsgAddress::AddressType getAddressType(const std::string &address);
            static std::string makeNormalizedNumber(const std::string &number);
            static bool isMms(Message::Type type);
            static bool isSms(Message::Type type);
    };
}

#endif /* MsgUtils_H_ */
