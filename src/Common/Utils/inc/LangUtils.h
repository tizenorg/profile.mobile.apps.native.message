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

#ifndef LangUtils_h_
#define LangUtils_h_

#include "Resource.h"
#include "Config.h"

#include <string>
#include <libintl.h>
#include <stdio.h>
#include <stdarg.h>

namespace Msg
{
    // Translatable Text
    struct TText
    {
        explicit TText(const char *stringId, const char *domainName)
            : m_pDomain(domainName)
            , m_pMsg(stringId)
        {
        };

        const char *getMsg() const
        {
            return m_pMsg;
        }

        const char *getDomain() const
        {
            return m_pDomain;
        }

    private:
        TText(TText&) = delete;
        TText operator =(TText&) = delete;

        const char *m_pDomain;
        const char *m_pMsg;
    };

    // dgettext
    struct DText
    {
        explicit DText(const char *stringId, const char *domainName)
        {
            m_pMsg = dgettext(domainName, stringId);
        }

        explicit DText(char dummyArg, const char *stringId, const char *domainName, ...)
            : m_pMsg(m_Buf)
        {
            va_list args;
            va_start (args, domainName);
            vsnprintf(m_Buf, maxBufSize, dgettext(domainName, stringId), args);
            va_end (args);
        }

        const char *get() const
        {
            return m_pMsg;
        }

        const char* cStr() const
        {
            return m_pMsg;
        }

        operator std::string() const
        {
            return m_pMsg;
        }

    private:
        DText(DText&) = delete;
        DText operator =(DText&) = delete;

        static const int maxBufSize = 1024;
        char m_Buf[maxBufSize];
        const char *m_pMsg;
    };

    #define msgt(strId) TText(strId, MSG_DOMAIN)
    #define msg(strId) DText(strId, MSG_DOMAIN)
    #define msgArgs(strId, ...) DText(0, strId, MSG_DOMAIN, __VA_ARGS__)
    #define sys(strId) DText(strId, SYS_DOMAIN)
    #define syst(strId) TText(strId, SYS_DOMAIN)
    #define sysArgs(strId, ...) DText(0, strId, SYS_DOMAIN, __VA_ARGS__)
}

#endif /* LangUtils_h_ */
