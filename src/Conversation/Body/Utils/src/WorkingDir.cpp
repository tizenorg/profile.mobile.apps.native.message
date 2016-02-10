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

#include "WorkingDir.h"
#include "PathUtils.h"
#include "FileUtils.h"
#include "Logger.h"

#include <string>
#include <fstream>

using namespace Msg;

const std::string commonWorkingDirName = "msg-composer";
const std::string currentWorkingDirPrefix = "composer";
const std::string textFileName = "mms";


WorkingDir::WorkingDir()
{
    createWorkingDir();
}

WorkingDir::~WorkingDir()
{
    removeWorkingDir();
}

void WorkingDir::createWorkingDir()
{
    // Create common dir. if not exists:
    std::string commonDir = PathUtils::getDataPath(commonWorkingDirName);
    bool isCommonExists = FileUtils::isExists(commonDir);
    if(!isCommonExists)
    {
        isCommonExists = FileUtils::makeDir(commonDir);
        MSG_ASSERT(isCommonExists, "Can't create common working dir: ", commonDir);
    }

    // Create current dir:
    if(isCommonExists)
    {
        std::string uniqueDir = getUniqueDirName(commonDir + "/" + currentWorkingDirPrefix);
        FileUtils::makeDir(uniqueDir);
        bool isExists = FileUtils::isExists(uniqueDir);
        MSG_ASSERT(isExists, "Can't create current working dir: ", uniqueDir);
        if(isExists)
        {
            m_Path = uniqueDir;
        }
    }
}

void WorkingDir::removeWorkingDir()
{
    if(!m_Path.empty())
    {
        bool res = FileUtils::remove(m_Path, true);
        MSG_LOG("Working dir remove with result = ", res);
    }
}

bool WorkingDir::isValid() const
{
    return !m_Path.empty() && FileUtils::isExists(m_Path);
}

const std::string &WorkingDir::getPath() const
{
    return m_Path;
}

std::string WorkingDir::addFile(const std::string &path)
{
    std::string newPath;

    if(FileUtils::isExists(path))
    {
        newPath = getNewFilePath(path);
        if(FileUtils::copy(path, newPath))
        {
            MSG_LOG("File added: ", newPath);
        }
    }

    return newPath;
}

std::string WorkingDir::addTextFile(const std::string &text)
{
    std::string path = getNewFilePath(textFileName);
    std::ofstream file(path, std::ofstream::trunc | std::ofstream::binary | std::ofstream::out);
    if(file.is_open())
    {
        file << text;
        MSG_LOG("Text file added: ", path);
    }
    else
    {
        path.clear();
    }

    return path;
}

bool WorkingDir::write(const std::string &path, const std::string &text)
{
    std::ofstream file(path, std::ofstream::trunc | std::ofstream::binary | std::ofstream::out);
    if(file.is_open())
       file << text;
    return file.is_open();
}

void WorkingDir::removeFile(const std::string &path)
{
    FileUtils::remove(path);
}

void WorkingDir::clear()
{
    FileUtils::remove(m_Path, false);
}

std::string WorkingDir::getNewFilePath(const std::string &path)
{
    std::string res = path;

    std::string base;
    std::string name;
    std::string ext;
    FileUtils::splitPath(path, base, name, ext);

    unsigned i = 0;
    do
    {
        res = m_Path + '/' + name;
        if(i > 0)
            res += "-" + std::to_string(i);
        if(!ext.empty())
            res += '.' + ext;
        ++i;
    }
    while(FileUtils::isExists(res));
    return res;
}

std::string WorkingDir::getUniqueDirName(const std::string &path)
{
    std::string res;
    unsigned i = 0;
    do
    {
        res = path + "-" + std::to_string(i);
        ++i;
    }
    while(FileUtils::isExists(res));
    return res;
}
