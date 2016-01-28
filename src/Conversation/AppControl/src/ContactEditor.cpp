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

#include "ContactEditor.h"
#include "Logger.h"
#include "MsgUtils.h"
#include "AppControlUtils.h"

using namespace Msg;

namespace
{
    const char* mimeContact = "application/vnd.tizen.contact";
}

ContactEditor::ContactEditor()
    : m_pListener(nullptr)
    , m_Handle()
{
}

ContactEditor::~ContactEditor()
{
    terminate();
}

void ContactEditor::setListener(IContactEditorListener *l)
{
    m_pListener = l;
}

bool ContactEditor::launch(const std::string &address, Operation operation)
{
    bool res = false;
    if(!address.empty())
    {
        const char *op = nullptr;
        if(operation == EditOp)
            op = APP_CONTROL_OPERATION_EDIT;
        else if(operation == CreateOp)
            op = APP_CONTROL_OPERATION_ADD;

        if(op)
        {
            if(MsgUtils::isValidNumber(address))
                res = launch(address, op, APP_CONTROL_DATA_PHONE);
            else if(MsgUtils::isValidEmail(address))
                res = launch(address, op, APP_CONTROL_DATA_EMAIL);
        }
    }
    return res;
}

bool ContactEditor::launch(const std::string &address, const char *operation, const char *addressType)
{
    bool res = false;
    terminate();
    if(APP_CONTROL_ERROR_NONE == app_control_create(&m_Handle))
    {
        app_control_set_operation(m_Handle, operation);
        app_control_set_mime(m_Handle, mimeContact);
        app_control_add_extra_data(m_Handle, addressType, address.c_str());
        app_control_set_launch_mode(m_Handle, APP_CONTROL_LAUNCH_MODE_GROUP);
        int ret = app_control_send_launch_request(m_Handle, APP_CONTROL_RES_CALLBACK(ContactEditor, onResultCb), this);
        MSG_LOG("Result code: ", ret);
        res = ret == APP_CONTROL_ERROR_NONE;
        if(!res)
            terminate();
    }

    return res;
}

void ContactEditor::terminate()
{
    if(m_Handle)
    {
        app_control_send_terminate_request(m_Handle);
        app_control_destroy(m_Handle);
        m_Handle = nullptr;
    }
}

void ContactEditor::onResultCb(app_control_h request, app_control_h reply, app_control_result_e result)
{
    if(m_Handle)
    {
        if(result == APP_CONTROL_RESULT_SUCCEEDED && m_pListener)
        {
            char *op = nullptr;
            app_control_get_operation(m_Handle, &op);

            if(op)
            {
                terminate();
                if(strcmp(op, APP_CONTROL_OPERATION_EDIT) == 0)
                    m_pListener->onContactChanged(*this);
                else if(strcmp(op, APP_CONTROL_OPERATION_ADD) == 0)
                    m_pListener->onContactCreated(*this);
                free(op);
            }
        }
        terminate();
    }
}
