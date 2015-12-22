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

#include "ResourceUtils.h"
#include "Logger.h"

#include <stdlib.h>
#include <app.h>

#ifdef _SAVE_IN_USER_SHARE_DIR_
#include <tzplatform_config.h>
#endif

using namespace Msg;


std::string ResourceUtils::getResourcePath(const std::string &filePath)
{
    std::string res;
    char *absolutePath = app_get_resource_path();

    res += absolutePath;
    res += filePath;
    free(absolutePath);

    return res;
}

std::string ResourceUtils::getDataPath(const std::string &filePath)
{
    std::string res;
    char *absolutePath = app_get_data_path();

    res += absolutePath;
    res += filePath;
    free(absolutePath);

    return res;
}

std::string ResourceUtils::getSharedTrustedPath(const std::string &filePath)
{
    std::string res;
#ifdef _SAVE_IN_USER_SHARE_DIR_
    res = tzplatform_mkpath(TZ_USER_SHARE, filePath.c_str());
#else
    char *absolutePath = app_get_shared_trusted_path();
    res += absolutePath;
    res += filePath;
    free(absolutePath);
#endif
    return res;
}
