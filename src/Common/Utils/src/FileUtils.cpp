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

#include "FileUtils.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

using namespace Msg;


long long FileUtils::getFileSize(const std::string &file)
{
    long long size = 0;

    struct stat st = {};
    if(stat(file.c_str(), &st) == 0)
    {
        size = st.st_size;
    }

    return size;
}

bool FileUtils::isExists(const std::string &file)
{
    bool res = false;
    struct stat st = {};
    res = stat(file.c_str(), &st) == 0 || file == "/";
    return res;
}

bool FileUtils::copy(const std::string &src, const std::string &dst)
{
    FILE *f1 = nullptr;
    FILE *f2 = nullptr;

    const size_t bufSize  = 4 * 1024;
    char buf[bufSize];
    char realPath1[PATH_MAX];
    char realPath2[PATH_MAX];

    size_t num = 0;
    bool ret = true;

    if(!realpath(src.c_str(), realPath1))
    {
        return false;
    }

    if(realpath(dst.c_str(), realPath2) && !strcmp(realPath1, realPath2))
    {
        return false;
    }

    f1 = fopen(src.c_str(), "rb");
    if(!f1)
    {
        return false;
    }

    f2 = fopen(dst.c_str(), "wb");
    if(!f2)
    {
        fclose(f1);
        return false;
    }

    while((num = fread(buf, 1, sizeof(buf), f1)) > 0)
    {
        if(fwrite(buf, 1, num, f2) != num)
        {
            ret = false;
        }
    }

    fclose(f1);
    fclose(f2);

    return ret;
}

bool FileUtils::makeDir(const std::string &dir)
{
    bool res = false;

    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    res = mkdir(dir.c_str(), mode) == 0;

    return res;
}

bool FileUtils::isDir(const std::string &file)
{
    bool res = false;

    struct stat st = {};
    if (stat(file.c_str(), &st) == 0)
    {
        res = S_ISDIR(st.st_mode);
    }

    return res;
}

std::string FileUtils::stripExtension(const std::string &path)
{
    auto pos = path.find_last_of(".");
    return pos == std::string::npos ? path : path.substr(0, pos);
}

bool FileUtils::remove(const std::string &path)
{
    bool res = true;
    struct dirent *ep = nullptr;
    DIR *dp = opendir(path.c_str());

    if(dp)
    {
        while((ep = readdir(dp)) != nullptr)
        {
            if(strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
            {
                std::string child = path + "/" + ep->d_name;
                res &= remove(child);
            }
        }

        closedir(dp);
        res &= rmdir(path.c_str()) == 0;
    }
    else
    {
        res &= unlink(path.c_str()) == 0;
    }

    return res;
}
