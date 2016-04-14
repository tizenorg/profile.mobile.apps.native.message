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

#include "MsgUtils.h"

#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <regex>

using namespace Msg;

namespace
{
    unsigned emailAddrLenMax = 256;
    const char *delimiters = ",;";
    const char *numberRegex = "\\+?([[:digit:]][ *\\(\\)-]*){2,14}[[:digit:]]$";
    const char *emailRegex = "(([[:alnum:]_!#$%&'*+/=?`{|}~^-]+)|(^\"([[:alnum:]_!#$%&'*+/=?`{|}~^-\\(\\)<>\\[\\]:;@,. (\\\\)(\\\")])+\"))+(?:\\.(([[:alnum:]_!#$%&'*+/=?`{|}~^-]+)|(\"([[:alnum:]_!#$%&'*+/=?`{|}~^-\\(\\)<>\\[\\]:;@,. (\\\\)(\\\")])+\")))*@[[:alnum:]-]+(?:\\.[[:alnum:]-]+)*";
}

TokenizedRecipients MsgUtils::tokenizeRecipients(const std::string &inputText)
{
    TokenizedRecipients result;
    std::string::size_type begin = inputText.find_first_not_of(delimiters);
    std::string::size_type end = inputText.find_first_of(delimiters, begin);
    std::string token;
    MsgAddress::AddressType addressType = MsgAddress::UnknownAddressType;

    while(begin != end)
    {
        if(begin != std::string::npos)
        {
            token = inputText.substr(begin, end != std::string::npos ? end-begin : std::string::npos);
            begin = inputText.find_first_not_of(delimiters, end);
            result.invalidResult.append(token);
            addressType = getAddressType(result.invalidResult);
            if(addressType != MsgAddress::UnknownAddressType)
            {
                result.validResults.push_back(std::make_pair(result.invalidResult, addressType));
                result.invalidResult.clear();
            }
            else if(end != std::string::npos)
            {
                result.invalidResult.append(inputText.substr(end, begin != std::string::npos ? begin-end : std::string::npos));
            }
            end = inputText.find_first_of(delimiters, begin);
        }
    }

    return result;
}

bool MsgUtils::isValidNumber(const std::string &address)
{
    std::regex numTemplate(numberRegex);
    return std::regex_match(address,numTemplate);
}

bool MsgUtils::isValidEmail(const std::string &address)
{
    if(address.length() > emailAddrLenMax)
    {
        return false;
    }
    std::regex emailTemplate(emailRegex);
    return std::regex_match(address,emailTemplate);
}

MsgAddress::AddressType MsgUtils::getAddressType(const std::string &address)
{
    if(isValidNumber(address))
    {
        return MsgAddress::Phone;
    }

    if(isValidEmail(address))
    {
        return MsgAddress::Email;
    }

    return MsgAddress::UnknownAddressType;
}

std::string MsgUtils::makeNormalizedNumber(const std::string &number)
{
    std::string normalizedNumber;
    for(auto symbol : number)
    {
        if(isdigit(symbol) || symbol == '*' || symbol == '#' || symbol == '+')
        {
            normalizedNumber += symbol;
        }
    }
    return normalizedNumber;
}

bool MsgUtils::isMms(Message::Type type)
{
    switch(type)
    {
        case Message::MT_MMS:
        case Message::MT_MMS_Noti:
            return true;
        default:
            return false;
    }
}

bool MsgUtils::isSms(Message::Type type)
{
    return type == Message::MT_SMS;
}
