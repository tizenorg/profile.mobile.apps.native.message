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

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Config.h"
#include "LoggerImpl.h"
#include <assert.h>

#ifndef  LOGGER_TAG
  #error "Please define LOGGER_TAG"
#endif

#define TRACE\
        LoggerImpl __tmp_logger_unique(LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__);\

#define MSG_LOG(...)\
        logTuple(LogPriority::INFO, LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

#define MSG_LOG_INFO(...)\
        logTuple(LogPriority::INFO, LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

#define MSG_LOG_ERROR(...)\
        logTuple(LogPriority::ERROR, LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

#define MSG_LOG_WARN(...)\
        logTuple(LogPriority::WARN, LOGGER_TAG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

#define MSG_ASSERT(expr, ...)\
        if(!(expr)) { MSG_LOG_ERROR(__VA_ARGS__); } \
        assert(expr)

#endif /* LOGGER_H_ */
