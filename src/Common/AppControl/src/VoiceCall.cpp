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

#include "VoiceCall.h"
#include <app_control.h>
#include "Logger.h"

using namespace Msg;

namespace
{
    const std::string tel = "tel:";
}

VoiceCall::VoiceCall()
{
}

bool VoiceCall::launch(const std::string &number)
{
    bool res = false;
    app_control_h svcHandle = nullptr;

    if(APP_CONTROL_ERROR_NONE == app_control_create(&svcHandle))
    {
        app_control_set_operation(svcHandle, APP_CONTROL_OPERATION_CALL);
        std::string phoneNumber = tel + number;

        app_control_set_uri(svcHandle, phoneNumber.c_str());
        int ret = app_control_send_launch_request(svcHandle, nullptr, nullptr);
        MSG_LOG("Result code: ", ret);
        res = ret == APP_CONTROL_ERROR_NONE;
        app_control_destroy(svcHandle);
    }
    return res;
}
