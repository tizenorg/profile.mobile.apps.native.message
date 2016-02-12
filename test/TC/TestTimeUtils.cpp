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
#include <utils_i18n_ulocale.h>

class TimeUtilsTest
    : public Msg::TimeUtils
{
    public:
        using TimeUtils::getDefaultLocale;
        using TimeUtils::getDateBestPattern;
        using TimeUtils::getFormattedDate;
};

using namespace Msg;

TEST( TimeUtilsTest, ValidTime )
{
    std::string systemLocale = TimeUtilsTest::getDefaultLocale(); // Save system locale before test start
    std::string localeTest = "en_";
    i18n_ulocale_set_default(localeTest.c_str());

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

    ASSERT_EQ("10:00", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "Hm"), time));
    ASSERT_EQ("10:00 AM", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "hma"), time));

    ASSERT_EQ("01/01/1970", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "MM/dd/yyyy"), time));
    ASSERT_NE("15/06/2016", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "MM/dd/yyyy"), time));

    ASSERT_EQ("Jan 01, 1970, 10:00", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "MMM/dd/yyyy HHmm"), time));
    ASSERT_NE("Dec 20, 2015, 15:49", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "MMM/dd/yyyy HHmm"), time));

    ASSERT_EQ("Jan 01, 1970, 10:00 AM", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "MMM/dd/yyyy hma"), time));
    ASSERT_NE("Feb 12, 1992, 05:45 PM", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "MMM/dd/yyyy hma"), time));

    ASSERT_EQ("Jan 1, 10:00", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "HH:mm, d MMM"), time));
    ASSERT_NE("Feb 5, 23:15", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "HH:mm, d MMM"), time));

    ASSERT_EQ("Jan 1, 10:00 AM", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "hh:mm a, d MMM"), time));
    ASSERT_NE("Dec 31, 08:42 PM", TimeUtilsTest::getFormattedDate(localeTest, TimeUtilsTest::getDateBestPattern(localeTest, "hh:mm a, d MMM"), time));

    i18n_ulocale_set_default(systemLocale.c_str());
}
