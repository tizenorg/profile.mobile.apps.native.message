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

#include "PathUtils.h"
#include "Logger.h"

#include <stdlib.h>
#include <app.h>

using namespace Msg;

std::string PathUtils::getResourcePath(const std::string &filePath)
{
    std::string res;
    char *absolutePath = app_get_resource_path();

    res += absolutePath;
    res += filePath;
    free(absolutePath);

    return res;
}

std::string PathUtils::getDataPath(const std::string &filePath)
{
    std::string res;
    char *absolutePath = app_get_data_path();

    res += absolutePath;
    res += filePath;
    free(absolutePath);

    return res;
}

std::string PathUtils::getResourcePath()
{
    std::string res;
    char* resPath = app_get_resource_path();
    if(resPath)
    {
        res = resPath;
        free(resPath);
    }

    return res;
}

std::string PathUtils::getLocalePath()
{
    std::string resPath(getResourcePath());
    if(!resPath.empty())
    {
        resPath.append("locale");
    }

    return resPath;
}
