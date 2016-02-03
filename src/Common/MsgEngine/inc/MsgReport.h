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

#ifndef MsgReport_h_
#define MsgReport_h_

#include "MsgList.h"

#include <memory>
#include <string>

namespace Msg
{
    class MsgReport;
    typedef MsgList<MsgReport> MsgReportList;
    typedef std::shared_ptr<MsgList<MsgReport>> MsgReportListRef;

    class MsgReport
    {
        public:
            enum DeliveryStatus
            {
                StatusNone,
                StatusExpired,
                StatusSuccess,
                StatusRejected
            };

            enum ReadStatus
            {
                ReadStatusNone,
                ReadStatusIsRead,
                ReadStatusIsDeleted,
                ReadStatusRejectByUser
            };

            enum Type
            {
                 TypeDelivery,
                 TypeRead,
                 TypeReadReportSent
            };

        public:
            virtual ~MsgReport() {};

            virtual std::string getAddress() const = 0;
            virtual DeliveryStatus getDeliveryStatus() const = 0;
            virtual ReadStatus getReadStatus() const = 0;
            virtual Type getType() const = 0;
            virtual time_t getTime() const = 0;
    };

}

#endif /* MsgReport_h_ */
