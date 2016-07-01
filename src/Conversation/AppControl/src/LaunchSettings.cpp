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

#include "LaunchSettings.h"
#include <app.h>

using namespace Msg;

LaunchSettings::LaunchSettings()
{
}

LaunchSettings::~LaunchSettings()
{
}

void LaunchSettings::launchSettings()
{
    app_control_h h;
    app_control_create(&h);
    app_control_set_operation(h, APP_CONTROL_OPERATION_SETTING);
    app_control_set_app_id(h, "org.tizen.setting");
    app_control_send_launch_request(h, NULL, NULL);
    app_control_destroy(h);
}
