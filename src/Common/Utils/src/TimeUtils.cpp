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

#include "TimeUtils.h"
#include "Logger.h"
#include <system_settings.h>
#include <unistd.h>
/* for i18n */
#include <utils_i18n_ustring.h>
#include <utils_i18n_ulocale.h>
#include <utils_i18n_udatepg.h>
#include <utils_i18n_udate.h>
#include <utils_i18n_ucalendar.h>

using namespace Msg;

namespace
{
    const int maxUcharLen = 64;
    const int maxCharLen = 128;
    const int defBufLen = 512;
    const std::string time12H = "hma";
    const std::string time24H = "Hm";
    const std::string dateDefault = "EEEMMMdd";
    const std::string dateYear = "EEEMMMddyyyy";
    const std::string dateListDefault = "MMdd";
    const std::string dateListYear = "MMddyyyy";
    const std::string dateTimeSceleton = "MM/dd/yyyy"; //readReport has the same format as Send Received time
    const std::string delivReportSms24H = "MMM/dd/yyyy HHmm"; //SMS Delivery report 24H format
    const std::string delivReportSms12H = "MMM/dd/yyyy hma"; //SMS Delivery report 12H format
    const std::string delivReportMms24H = "HH:mm, d MMM"; //MMS Delivery report 24H format
    const std::string delivReportMms12H = "hh:mm a, d MMM"; //MMS Delivery report 12H format
}

std::string TimeUtils::makeThreadTimeString(time_t msgTime)
{
    tm msgTimeTm;
    tm curTimeTm;
    time_t curTime;
    bool isDayDif = false;
    bool isYearDif = false;
    int msgDay = 0;
    int curDay = 0;
    std::string locale = getDefaultLocale();

    curTime = (time_t)time(nullptr);

    localtime_r(&msgTime, &msgTimeTm);
    localtime_r(&curTime, &curTimeTm);

    msgDay = (msgTimeTm.tm_year - 1900) * 365 + msgTimeTm.tm_yday + (msgTimeTm.tm_year - 1900 - 1) / 4;
    curDay = (curTimeTm.tm_year - 1900) * 365 + curTimeTm.tm_yday + (curTimeTm.tm_year - 1900 - 1) / 4;

    isDayDif = (curDay - msgDay == 0);
    isYearDif = (msgTimeTm.tm_year - curTimeTm.tm_year != 0);

    if(isDayDif)
    {
        if(getTimeFormat() == TimeFormat24H)
            return getFormattedDate(locale, getDateBestPattern(locale, time24H), msgTime);
        else
            return getFormattedDate(locale, getDateBestPattern(locale, time12H), msgTime);
    }
    else
    {
        if(isYearDif)
            return getFormattedDate(locale, getDateBestPattern(locale, dateListYear), msgTime);
        else
            return getFormattedDate(locale, getDateBestPattern(locale, dateListDefault), msgTime);
    }
}

std::string TimeUtils::makeBubbleTimeString(time_t msgTime)
{
    std::string locale = getDefaultLocale();

    return getTimeFormat() == TimeFormat24H
            ? getFormattedDate(locale, getDateBestPattern(locale, time24H), msgTime)
            : getFormattedDate(locale, getDateBestPattern(locale, time12H), msgTime);
}

std::string TimeUtils::makeBubbleDateLineString(time_t msgTime)
{
    std::string locale = getDefaultLocale();
    return getFormattedDate(locale, getDateBestPattern(locale, dateYear), msgTime);
}

std::string TimeUtils::makeDateTimeString(time_t msgTime) //the same for Sim Message List
{
    std::string locale = getDefaultLocale();
    std::string res = getFormattedDate(locale, getDateBestPattern(locale, dateTimeSceleton), msgTime);

    res.append(" ");
    if(getTimeFormat() == TimeFormat24H)
        res += getFormattedDate(locale, getDateBestPattern(locale, time24H), msgTime);
    else
        res += getFormattedDate(locale, getDateBestPattern(locale, time12H), msgTime);

    return res;
}
std::string TimeUtils::makeSmsReportTimeString(time_t msgTime)
{
    std::string locale = getDefaultLocale();
    return getTimeFormat() == TimeFormat24H
            ? getFormattedDate(locale, getDateBestPattern(locale, delivReportSms24H), msgTime)
            : getFormattedDate(locale, getDateBestPattern(locale, delivReportSms12H), msgTime);
}

std::string TimeUtils::makeMmsReportTimeString(time_t msgTime)
{
    std::string locale = getDefaultLocale();
    return getTimeFormat() == TimeFormat24H
            ? getFormattedDate(locale, getDateBestPattern(locale, delivReportMms24H), msgTime)
            : getFormattedDate(locale, getDateBestPattern(locale, delivReportMms12H), msgTime);
}

int TimeUtils::getTimeFormat()
{
    bool timeFormat = false;

    if (system_settings_get_value_bool(SYSTEM_SETTINGS_KEY_LOCALE_TIMEFORMAT_24HOUR, &timeFormat) < 0)
        return TimeFormatUnknown;

    return timeFormat ? TimeFormat24H : TimeFormat12H;
}

std::string TimeUtils::getDefaultLocale()
{
    const char *locale = nullptr;

    i18n_ulocale_set_default(getenv("LC_TIME"));
    i18n_ulocale_get_default(&locale);
    strtok((char *)locale, ".UTF8");

    return locale ? locale : "";
}

std::string TimeUtils::getTimezone()
{
    char buf[defBufLen] = {0,};
    ssize_t len = readlink("/opt/etc/localtime", buf, sizeof(buf) - 1);

    if (len != -1)
    {
        buf[len] = '\0';
    }
    else
    {
        MSG_LOG_ERROR("Getting default timezone failed, use system settings value");
        char *time_zone = nullptr;
        system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_TIMEZONE, &time_zone);
        return time_zone ? time_zone : "";
    }

    return strlen(buf) != 0 ? buf + 20 : "";
}

std::string TimeUtils::getDateBestPattern(std::string &locale, const std::string skeleton)
{
    i18n_udatepg_h generator = nullptr;
    i18n_uchar bestPattern[maxUcharLen + 1] = {0,};
    char bestPatternString[maxCharLen + 1] = {0,};
    int32_t bestPatternCapacity = maxUcharLen;
    int bestPatternLen = 0;
    i18n_uchar customSkeleton[maxUcharLen + 1] = {0,};
    int skeletonLength = 0;

    skeletonLength = skeleton.length();
    /* convert char to uchar */
    if (skeletonLength <= maxUcharLen)
        i18n_ustring_copy_ua_n(customSkeleton, skeleton.c_str(), skeletonLength);
    else
        i18n_ustring_copy_ua_n(customSkeleton, skeleton.c_str(), maxUcharLen);

    i18n_udatepg_create(locale.c_str(), &generator);

    /* get bestpattern from customskeletom of uchar */
    skeletonLength = i18n_ustring_get_length(customSkeleton);
    i18n_udatepg_get_best_pattern(generator, customSkeleton, skeletonLength, bestPattern, bestPatternCapacity, &bestPatternLen);
    /* convert uchar to char to know bestPattern */
    i18n_ustring_copy_au(bestPatternString, bestPattern);

    i18n_udatepg_destroy(generator);

    return strlen(bestPatternString) != 0 ? bestPatternString : "";
}

std::string TimeUtils::getFormattedDate(std::string &locale, std::string bestPattern, time_t time)
{
    int status = I18N_ERROR_NONE;
    i18n_udate date;
    i18n_udate_format_h dateFormat = nullptr;
    i18n_uchar utimezoneId[defBufLen] = {0,};
    i18n_uchar uBestPattern[maxUcharLen + 1] = {0,};
    i18n_uchar formatted[maxUcharLen + 1] = {0,};
    char formattedString[maxCharLen + 1] = {0,};
    int32_t formattedCapacity = maxUcharLen;
    int formattedLength = 0;
    int patternLength = bestPattern.length();;

    /* convert char to uchar */
    if (patternLength <= maxUcharLen)
        i18n_ustring_copy_ua_n(uBestPattern, bestPattern.c_str(), patternLength);
    else
        i18n_ustring_copy_ua_n(uBestPattern, bestPattern.c_str(), maxUcharLen);

    /* get current timezone and set as default timezone */
    i18n_ustring_copy_ua(utimezoneId, getTimezone().c_str());

    status = i18n_ucalendar_set_default_timezone(utimezoneId);
    if (status != I18N_ERROR_NONE)
        MSG_LOG_ERROR("default timezone set error");

    /* convert time_t to UDate, if it is */
    if(time)
        date = (i18n_udate)time * 1000;
    else
        i18n_ucalendar_get_now(&date);

    dateFormat = getDateFormat(locale, uBestPattern, &status);
    if(status == I18N_ERROR_NONE)
    {
        /* get formatted string */
        i18n_udate_format_date(dateFormat, date, formatted, formattedCapacity, nullptr, &formattedLength);
        /* convert uchar to char */
        i18n_ustring_copy_au(formattedString, formatted);
    }

    return strlen(formattedString) != 0 ? formattedString : "";
}

i18n_udate_format_h TimeUtils::getDateFormat(std::string &locale, const i18n_uchar *pattern, int *status)
{
    i18n_udate_format_h dateFormat = nullptr;

    *status = i18n_udate_create(I18N_UDATE_PATTERN, I18N_UDATE_PATTERN, locale.c_str(), nullptr, -1, pattern, -1, &dateFormat);

    return dateFormat;
}
