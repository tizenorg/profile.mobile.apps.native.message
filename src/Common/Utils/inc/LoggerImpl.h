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

#ifndef LOGGERIMPL_H_
#define LOGGERIMPL_H_

#include <string>
#include <tuple>
#include <sstream>
#include <dlog.h>

enum class LogPriority
{
    DEFAULT = DLOG_DEFAULT,
    VERBOSE = DLOG_VERBOSE, /**< Verbose */
    DEBUG = DLOG_DEBUG, /**< Debug */
    INFO = DLOG_INFO, /**< Info */
    WARN = DLOG_WARN, /**< Warning */
    ERROR = DLOG_ERROR, /**< Error */
    FATAL = DLOG_FATAL, /**< Fatal */
    SILEN = DLOG_SILENT, /**< Silent */
    MAX = DLOG_PRIO_MAX/**< Keep this always at the end. */
};

template <size_t N>
struct SizeT
{
};

template < typename TupleType>
void iterateTuple(std::stringstream & stream, TupleType & tuple)
{
    iterateTuple(stream, tuple, SizeT<std::tuple_size<TupleType>::value>());
}

template <typename TupleType>
void iterateTuple(std::stringstream & stream, TupleType& tuple, SizeT<0> )
{
}

template <typename TupleType, size_t N >
void iterateTuple(std::stringstream & stream, TupleType& tuple, SizeT<N>)
{
    iterateTuple(stream, tuple, SizeT<N-1>());
    stream<<std::get<N-1>(tuple);
}

template<typename... Tail>
void logTuple( LogPriority prior
             , const char *tag
             , const char * file, const char * function, int line
             , Tail... msg)
{
    std::stringstream messageStr;
    std::tuple<Tail...> tuple = std::make_tuple(msg...);
    iterateTuple(messageStr, tuple);
    dlog_print( static_cast<log_priority>(prior)
              , tag, "%s: %s(%d) -> %s", file, function, line
              , messageStr.str().c_str());
}

class LoggerImpl
{
    public:
        LoggerImpl(const char *tag, const char * file, const char * function, int line);
        ~LoggerImpl();
    private:
        const std::string mtag;
        const std::string mfile;
        const std::string mfunction;
};

#endif /* LOGGERIMPL_H_ */
