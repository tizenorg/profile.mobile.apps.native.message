/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __MSG_TIME_UTIL_H__
#define __MSG_TIME_UTIL_H__

#include <string>
#include <utils_i18n_types.h>

namespace Msg
{
    class TimeUtils
    {
        enum TimeFormat
        {
            TimeFormatUnknown,
            TimeFormat24H,
            TimeFormat12H

        };

        public:
            static std::string makeThreadTimeString(time_t msgTime);
            static std::string makeBubbleTimeString(time_t msgTime);
            static std::string makeBubbleDateLineString(time_t msgTime);
            static std::string makeDateTimeString(time_t msgTime);
            static std::string makeSmsReportTimeString(time_t msgTime);
            static std::string makeMmsReportTimeString(time_t msgTime);

        protected:
            //made protected to allow inheritance of this class in tests
            static std::string getDefaultLocale();
            static std::string getDateBestPattern(std::string &locale, const std::string skeleton);
            static std::string getFormattedDate(std::string &locale, std::string bestPattern, time_t time);

        private:
            static int getTimeFormat();
            static std::string getTimezone();
            static i18n_udate_format_h getDateFormat(std::string &locale, const i18n_uchar *pattern, int *status);
    };
}

#endif /* __MSG_TIME_UTIL_H__ */
