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
#include <metadata_extractor.h>
#include <unistd.h>
#include <regex>

using namespace Msg;

namespace
{
    unsigned emailAddrLenMax = 256;
    unsigned addrLenMin = 1;
    const unsigned numberLenMin = 3;
    const unsigned numberLenMax = 15;
    const char *delimiters = " ,\n\r\t;";
    const char *numberRegex = "\\+?([[:digit:]][ *\\(\\)-]*){2,14}[[:digit:]]$";
    const char *emailRegex = "(([[:alnum:]_!#$%&'*+/=?`{|}~^-]+)|(^\"([[:alnum:]_!#$%&'*+/=?`{|}~^-\\(\\)<>\\[\\]:;@,. (\\\\)(\\\")])+\"))+(?:\\.(([[:alnum:]_!#$%&'*+/=?`{|}~^-]+)|(\"([[:alnum:]_!#$%&'*+/=?`{|}~^-\\(\\)<>\\[\\]:;@,. (\\\\)(\\\")])+\")))*@[[:alnum:]-]+(?:\\.[[:alnum:]-]+)*";
}

std::list<std::string> MsgUtils::tokenizeRecipients(const std::string &inputText)
{
    std::list<std::string> res;
    char *str = strdup(inputText.c_str());
    char *next = strtok(str, delimiters);

    while(next)
    {
        res.push_back(next);
        next = strtok(NULL, delimiters);
    }

    free(str);
    return res;
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

MsgAddress MsgUtils::getAddressType(const std::string &address)
{
    if (isValidNumber(address))
        return MsgAddress::Number;

    if(isValidEmail(address))
        return MsgAddress::Email;

    return MsgAddress::Invalid;
}

std::string MsgUtils::getMediaTitle(const std::string &path)
{
    char *alert_title = nullptr;
    int ret = METADATA_EXTRACTOR_ERROR_NONE;
    metadata_extractor_h metadata = NULL;
    if (access( path.c_str(), F_OK ) < 0)
    {
        return path;
    }

    ret = metadata_extractor_create(&metadata);
    if (ret != METADATA_EXTRACTOR_ERROR_NONE)
    {
        return path;
    }

    ret = metadata_extractor_set_path(metadata, path.c_str());
    if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
        metadata_extractor_destroy(metadata);
        return path;
    }

    ret = metadata_extractor_get_metadata(metadata, METADATA_TITLE, &alert_title);
    if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
        metadata_extractor_destroy(metadata);
        return path;
    }

    metadata_extractor_destroy(metadata);
    return std::string(alert_title);
}

