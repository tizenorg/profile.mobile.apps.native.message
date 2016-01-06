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

#include "AppControlUtils.h"
#include <string>

using namespace Msg;

std::string AppControlUtils::getExtraData(app_control_h handle, const std::string &key)
{
    std::string res;
    char *val = nullptr;
    if(APP_CONTROL_ERROR_NONE == app_control_get_extra_data(handle, key.c_str(), &val))
    {
        if(val)
        {
            res = val;
            free(val);
        }
    }
    return res;
}

void AppControlUtils::getExtraDataArray(app_control_h handle, const std::string &key, std::list<std::string> &outArray)
{
    int arrayLength = 0;
    char **pArrayVal = nullptr;
    if(APP_CONTROL_ERROR_NONE == app_control_get_extra_data_array(handle, key.c_str(), &pArrayVal, &arrayLength))
    {
        if(arrayLength != 0)
        {
            std::copy(pArrayVal, pArrayVal + arrayLength, std::back_inserter(outArray));
            free(pArrayVal);
        }
    }
}

void AppControlUtils::getExtraDataIntArray(app_control_h handle, const std::string &key, std::list<int> &outArray)
{
    int arrayLength = 0;
    char **pArrayVal = nullptr;
    if(APP_CONTROL_ERROR_NONE == app_control_get_extra_data_array(handle, key.c_str(), &pArrayVal, &arrayLength)
            && pArrayVal)
    {
        std::string::size_type sz;
        for(int i=0; i<arrayLength; ++i)
        {
            if(pArrayVal[i])
            {
                std::string s(pArrayVal[i]);
                int parsed = std::stoi(s, &sz);
                if(sz == s.length())
                {
                    outArray.push_back(parsed);
                }
            }
        }
        free(pArrayVal);
    }
}
