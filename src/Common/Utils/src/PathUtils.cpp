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

#include <storage-expand.h>
#include <stdlib.h>
#include <app.h>

using namespace Msg;

namespace
{
    std::string download;
}

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

static bool storageCb(int storageId, storage_type_e type, storage_state_e state, const char *path, void *userData)
{
    if(type == STORAGE_TYPE_INTERNAL)
    {
        int *internalStorage = (int *)userData;
        *internalStorage = storageId;
        return false;
    }
    return true;
}

const std::string &PathUtils::getDownloadPath()
{
    if(!download.empty())
        return download;

    char *dirPath = nullptr;
    int storageId = -1;

    int error = storage_foreach_device_supported(storageCb, &storageId);
    if(error == STORAGE_ERROR_NONE)
        storage_get_directory(storageId, STORAGE_DIRECTORY_DOWNLOADS, &dirPath);

    if(dirPath)
    {
        download = dirPath;
        free(dirPath);
    }

    return download;
}
