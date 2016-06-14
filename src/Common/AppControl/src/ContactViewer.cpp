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

#include "ContactViewer.h"
#include "Logger.h"

#include <app_control.h>
#include <string>

using namespace Msg;

namespace
{
    const char *mimeContact = "application/vnd.tizen.contact";
    const char *personContactTypeStr = "person";
    const char *myProfileTypeStr = "my_profile";
}

bool ContactViewer::launch(int id, ContactAddress::OwnerType ownerType)
{
    bool res = false;
    app_control_h svc_handle = nullptr;

    if(APP_CONTROL_ERROR_NONE == app_control_create(&svc_handle))
    {
        app_control_set_operation(svc_handle, APP_CONTROL_OPERATION_VIEW);
        app_control_set_mime(svc_handle, mimeContact);
        app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_TYPE, toStr(ownerType));
        app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_ID, std::to_string(id).c_str());
        app_control_set_launch_mode(svc_handle, APP_CONTROL_LAUNCH_MODE_GROUP);
        int ret = app_control_send_launch_request(svc_handle, nullptr, nullptr);
        MSG_LOG("Result code: ", ret);
        res = ret == APP_CONTROL_ERROR_NONE;
        app_control_destroy(svc_handle);
    }

    return res;
}

bool ContactViewer::launch(const ContactAddress &address)
{
    return launch(address.getOwnerId(), address.getOwnerType());
}

const char *ContactViewer::toStr(ContactAddress::OwnerType type)
{
    switch(type)
    {
        case ContactAddress::PersonType:
            return personContactTypeStr;
        case ContactAddress::MyProfileType:
            return myProfileTypeStr;
        default:
            MSG_LOG_ERROR("Unknown ContactType");
    };
    return "";
}

