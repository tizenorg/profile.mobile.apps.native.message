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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "TimeUtils.h"
#include <time.h>
#include <string>

class TimeUtilsTest
    : public Msg::TimeUtils
{
    public:
        using TimeUtils::getTimeFormat;
        using TimeUtils::getDefaultLocale;
        using TimeUtils::getTimezone;
        using TimeUtils::getDateBestPattern;
        using TimeUtils::getFormattedDate;
        using TimeUtils::getDateFormat;
        using TimeUtils::TimeFormat;
};

using namespace Msg;
//int main()
TEST( TimeUtilsTest, ValidTime )
{
    std::string locale = TimeUtilsTest::getDefaultLocale();
    TimeUtilsTest::TimeFormat timeFormat = TimeUtilsTest::getTimeFormat();
    std::string sceleton;
    std::string res;

    struct tm generateTime;
    time_t time;

    generateTime.tm_sec = 0;            /* Seconds. [0-60] (1 leap second) */
    generateTime.tm_min = 0;            /* Minutes. [0-59] */
    generateTime.tm_hour = 10;          /* Hours.   [0-23] */
    generateTime.tm_mday = 1;           /* Day.     [1-31] */
    generateTime.tm_mon = 0;            /* Month.   [0-11] */
    generateTime.tm_year = 1970 - 1900; /* Year - 1900.  */
    generateTime.tm_wday = 3;           /* Day of week. [0-6] */
    time = mktime(&generateTime);

    ASSERT_EQ(TimeUtilsTest::getFormattedDate(locale, TimeUtilsTest::getDateBestPattern(locale, "MMddyyyy"), time), TimeUtilsTest::makeThreadTimeString(time));
    ASSERT_NE("02/06/2016", TimeUtilsTest::makeThreadTimeString(time));

    if(timeFormat == TimeUtilsTest::TimeFormat::TimeFormat24H)
        sceleton = "Hm";
    else
        sceleton = "hma";
    ASSERT_EQ(TimeUtilsTest::getFormattedDate(locale, TimeUtilsTest::getDateBestPattern(locale, sceleton), time), TimeUtilsTest::makeBubbleTimeString(time));
    ASSERT_NE("15:26", TimeUtilsTest::makeBubbleTimeString(time));

    res = TimeUtilsTest::getFormattedDate(locale, TimeUtilsTest::getDateBestPattern(locale, "MM/dd/yyyy"), time);
    res.append(" ");
    res += TimeUtilsTest::getFormattedDate(locale, TimeUtilsTest::getDateBestPattern(locale, sceleton), time);

    ASSERT_EQ(res, TimeUtilsTest::makeDateTimeString(time));
    ASSERT_NE("02/06/2016 15:26", TimeUtilsTest::makeDateTimeString(time));

    if(timeFormat == TimeUtilsTest::TimeFormat::TimeFormat24H)
        sceleton = "MMM/dd/yyyy HHmm";
    else
        sceleton = "MMM/dd/yyyy hma";
    ASSERT_EQ(TimeUtilsTest::getFormattedDate(locale, TimeUtilsTest::getDateBestPattern(locale, sceleton), time), TimeUtilsTest::makeSmsReportTimeString(time));
    ASSERT_NE("25 Feb 2017, 14:00", TimeUtilsTest::makeSmsReportTimeString(time));

    if(timeFormat == TimeUtilsTest::TimeFormat::TimeFormat24H)
        sceleton = "HH:mm, d MMM";
    else
        sceleton = "hh:mm a, d MMM";
    ASSERT_EQ(TimeUtilsTest::getFormattedDate(locale, TimeUtilsTest::getDateBestPattern(locale, sceleton), time), TimeUtilsTest::makeMmsReportTimeString(time));
    ASSERT_NE("15:06, 25 Dec", TimeUtilsTest::makeMmsReportTimeString(time));
}

