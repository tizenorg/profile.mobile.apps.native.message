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
 */

#include "MsgReportPrivate.h"
#include "MsgUtilsPrivate.h"

#include <msg.h>

using namespace Msg;

MsgReportPrivate::MsgReportPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , MsgReport()
{

}

MsgReportPrivate::~MsgReportPrivate()
{

}

std::string MsgReportPrivate::getAddress() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_REPORT_ADDRESS_STR, MAX_ADDRESS_VAL_LEN);
}

MsgReport::DeliveryStatus MsgReportPrivate::getDeliveryStatus() const
{
    int nativeType = MSG_DELIVERY_REPORT_NONE;
    msg_get_int_value(m_MsgStruct, MSG_REPORT_STATUS_INT, &nativeType);
    return MsgUtilsPrivate::nativeToReportDeliveryStatus(nativeType);
}

MsgReport::ReadStatus MsgReportPrivate::getReadStatus() const
{
    int nativeType = MSG_READ_REPORT_NONE;
    msg_get_int_value(m_MsgStruct, MSG_REPORT_STATUS_INT, (int *)&nativeType);
    return MsgUtilsPrivate::nativeToReportReadStatus(nativeType);
}

MsgReport::Type MsgReportPrivate::getType() const
{
    int nativeType = MSG_REPORT_TYPE_DELIVERY;
    msg_get_int_value(m_MsgStruct, MSG_REPORT_TYPE_INT, &nativeType);
    return MsgUtilsPrivate::nativeToReportType(nativeType);
}

time_t MsgReportPrivate::getTime() const
{
    int time = 0;
    msg_get_int_value(m_MsgStruct, MSG_REPORT_TIME_INT, &time);
    return (time_t)time;
}
