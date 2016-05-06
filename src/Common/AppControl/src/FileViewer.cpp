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

#include "FileViewer.h"
#include "Logger.h"
#include "FileUtils.h"

#include <app_control.h>

using namespace Msg;

FileViewer::FileViewer()
{
}

bool FileViewer::launch(const std::string &file)
{
    bool res = false;
    app_control_h svc_handle = nullptr;
    std::string mime = FileUtils::getMimeType(file);

    MSG_LOG("File path: ", file);
    MSG_LOG("MIME type: ", mime);

    if(mime.empty() || file.empty())
        return false;

    if(APP_CONTROL_ERROR_NONE == app_control_create(&svc_handle))
    {
        app_control_set_operation(svc_handle, APP_CONTROL_OPERATION_VIEW);
        app_control_set_mime(svc_handle, mime.c_str());
        app_control_set_uri(svc_handle, file.c_str());
        app_control_set_launch_mode(svc_handle, APP_CONTROL_LAUNCH_MODE_GROUP);
        int ret = app_control_send_launch_request(svc_handle, nullptr, nullptr);
        MSG_LOG("Result code: ", ret);
        res = ret == APP_CONTROL_ERROR_NONE;
        app_control_destroy(svc_handle);
    }

    return res;
}

