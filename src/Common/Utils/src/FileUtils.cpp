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
#include <fstream>

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
        return false;

    if(realpath(dst.c_str(), realPath2) && !strcmp(realPath1, realPath2))
        return false;

    f1 = fopen(src.c_str(), "rb");
    if(!f1)
        return false;

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

void FileUtils::splitPath(const std::string &path, std::string &basePath,
                          std::string &fileName, std::string &extension)
{
    auto pos = path.find_last_of('/');

    if(pos != std::string::npos)
        basePath = path.substr(0, pos + 1);

    std::string name;
    name = path.substr(pos + 1);

    pos = name.find_last_of('.');
    if(pos != std::string::npos)
    {
        extension = name.substr(pos + 1);
        if(pos != 0)
            fileName = name.substr(0, pos);
    }
    else
    {
        fileName = name;
    }
}

std::string FileUtils::getFileName(const std::string &path)
{
    auto pos = path.find_last_of('/');
    return pos != std::string::npos ? path.substr(pos + 1) : path;
}

bool FileUtils::remove(const std::string &path, bool removeCurrentDir)
{
    bool res = true;
    struct dirent ep;
    struct dirent *dirData = nullptr;
    DIR *dp = opendir(path.c_str());

    if(dp)
    {
        int readirRes = 0;
        while(true)
        {
            readirRes = readdir_r(dp, &ep, &dirData);
            if(readirRes != 0)
                return false;

            if(dirData == nullptr)
                break;

            if(strcmp(dirData->d_name, ".") != 0 && strcmp(dirData->d_name, "..") != 0)
            {
                std::string child = path + "/" + dirData->d_name;
                res &= remove(child, true);
            }
        }

        closedir(dp);
        if(removeCurrentDir)
        {
            res &= rmdir(path.c_str()) == 0;
        }
    }
    else
    {
        res &= unlink(path.c_str()) == 0;
    }

    return res;
}

std::string FileUtils::readTextFile(const std::string &path)
{
    std::ifstream fs(path, std::ifstream::in | std::ifstream::binary);
    return fs.is_open() ? std::string(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>()) : "";
}

bool FileUtils::writeTextFile(const std::string &path, const std::string &text)
{
    std::ofstream file(path, std::ofstream::trunc | std::ofstream::binary | std::ofstream::out);
    if(file.is_open())
       file << text;
    return file.is_open() && file.good();
}

std::string FileUtils::genUniqueFilePath(const std::string &storagePath, const std::string &filePath)
{
    std::string res = filePath;
    std::string base;
    std::string name;
    std::string ext;
    splitPath(filePath, base, name, ext);

    unsigned i = 0;
    do
    {
        res = storagePath;
        if(res.back() != '/')
            res += '/';

        res += name;
        if(i > 0)
            res += "-" + std::to_string(i);
        if(!ext.empty())
            res += '.' + ext;
        ++i;
    } while(isExists(res));
    return res;
}
