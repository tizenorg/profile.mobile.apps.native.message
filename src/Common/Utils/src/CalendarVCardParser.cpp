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

#include "CalendarVCardParser.h"
#include "FileUtils.h"
#include "Logger.h"
#include "TimeUtils.h"

#include <string.h>

using namespace Msg;

CalendarEvent::CalendarEvent(calendar_record_h record)
{
    char *summary = nullptr;
    calendar_record_get_str_p(record, _calendar_event.summary, &summary);
    if(summary)
        m_Summary = summary;

    calendar_time_s start = {};
    calendar_record_get_caltime(record, _calendar_event.start_time, &start);
    if(start.type == CALENDAR_TIME_UTIME) // Unix time
    {
        m_StartDate = TimeUtils::makeCalEventString(start.time.utime);
    }
    else // Local time
    {
        // FIXME: timezone is null
        char *timezone = nullptr;
        calendar_record_get_str_p(record, _calendar_event.timezone, &timezone);
        m_StartDate = TimeUtils::makeCalEventString
        (
            start.time.date.year,
            start.time.date.month,
            start.time.date.mday,
            start.time.date.hour,
            start.time.date.minute,
            timezone
        );
    }

}

const std::string &CalendarEvent::getSummary() const
{
    return m_Summary;
}

const std::string &CalendarEvent::getStartDate() const
{
    return m_StartDate;
}

CalendarVCardParser::CalendarVCardParser()
{
    calendar_connect();
}

CalendarVCardParser::~CalendarVCardParser()
{
    calendar_disconnect();
}

CalendarVCardParser &CalendarVCardParser::getInst()
{
    static CalendarVCardParser inst;
    return inst;
}

std::list<CalendarEvent> CalendarVCardParser::parse(const std::string &filePath)
{
    if(filePath.empty())
        return {};

    std::list<CalendarEvent> list;
    std::string text = FileUtils::readTextFile(filePath);

    calendar_list_h records = nullptr;
    int parseRes = calendar_vcalendar_parse_to_calendar(text.c_str(), &records);
    MSG_LOG("Parse results: ", parseRes);

    int count = 0;
    calendar_list_get_count(records, &count);
    calendar_list_first(records);

    for(int i = 0; i < count; ++i)
    {
        calendar_record_h record = nullptr;
        calendar_list_get_current_record_p(records, &record);
        if(record)
        {
            char *viewUri = nullptr;
            calendar_record_get_uri_p(record, &viewUri);
            if(viewUri && !strcmp(viewUri, _calendar_event._uri))
                list.emplace_back(record);
        }
    }

    if(records)
        calendar_list_destroy(records, true);

    return list;
}


