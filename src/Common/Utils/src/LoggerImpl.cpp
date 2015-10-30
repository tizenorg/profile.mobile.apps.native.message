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

#include "LoggerImpl.h"

namespace
{
    const std::string enterFraseFormat = "%s: %s(%d) -> [ENTER]";
    const std::string leaveFraseFormat = "%s: %s -> [LEAVE]";
    LogPriority defaultPriority = LogPriority::DEBUG;
}

LoggerImpl::LoggerImpl(const char *tag, const char * file, const char * function, int line)
    : mtag(tag)
    , mfile(file)
    , mfunction(function)
{
    dlog_print( static_cast<log_priority>(defaultPriority)
              , mtag.c_str()
              , enterFraseFormat.c_str()
              , mfile.c_str()
              , mfunction.c_str()
              , line);
}

LoggerImpl::~LoggerImpl()
{
    dlog_print( static_cast<log_priority>(defaultPriority)
              , mtag.c_str()
              , leaveFraseFormat.c_str()
              , mfile.c_str()
              , mfunction.c_str());
}
