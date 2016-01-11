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

#include "ContactPicker.h"
#include <app_control.h>
#include "Logger.h"

using namespace Msg;

namespace
{
    const char* mimeContact = "application/vnd.tizen.contact";
    const char* single  = "single";
    const char* multiple  = "multiple";
    const char* resultTypePhone  = "phone";
}

ContactPicker::ContactPicker()
    : m_pListener(nullptr)
{
}

void ContactPicker::setListener(IContactPickerListener *pListener)
{
    m_pListener = pListener;
}

bool ContactPicker::launch(size_t howManyToPick)
{
    bool res = false;
    app_control_h svc_handle = NULL;

    if(APP_CONTROL_ERROR_NONE == app_control_create(&svc_handle))
    {
        app_control_set_operation(svc_handle, APP_CONTROL_OPERATION_PICK);
        app_control_set_mime(svc_handle, mimeContact);
        if(howManyToPick > 1)
        {
            app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_SELECTION_MODE, multiple);
            app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_TOTAL_COUNT, std::to_string(howManyToPick).c_str());
        }
        else
        {
            app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_SELECTION_MODE, single);
        }

        //TODO: We have to request a possibility to acquire phone numbers and emails at the same time
        app_control_add_extra_data(svc_handle, APP_CONTROL_DATA_TYPE, resultTypePhone);
        int ret = app_control_send_launch_request(svc_handle, APP_CONTROL_RES_CALLBACK(ContactPicker, onAppControlRes), this);
        MSG_LOG("launching contacts, result-code: ", ret);
        res = (ret == APP_CONTROL_ERROR_NONE);

        app_control_destroy(svc_handle);
    }

    return res;
}

void ContactPicker::onAppControlRes(app_control_h request, app_control_h reply, app_control_result_e result)
{
    if(result == APP_CONTROL_RESULT_SUCCEEDED)
    {
        std::list<int> phoneNumIds;
        AppControlUtils::getExtraDataIntArray(reply, APP_CONTROL_DATA_SELECTED, phoneNumIds);
        if(m_pListener)
        {
            m_pListener->onContactsPicked(phoneNumIds);
        }
    }
}
