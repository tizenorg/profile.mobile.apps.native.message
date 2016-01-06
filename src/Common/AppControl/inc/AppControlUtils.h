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

#ifndef AppControlUtils_h_
#define AppControlUtils_h_

#include <app_control.h>
#include <string>
#include <list>

// Delete this defines after adding them to app_control header
#define APP_CONTROL_MIME_CONTACT "application/vnd.tizen.contact"

#define APP_CONTROL_DATA_RESULT_TYPE "http://tizen.org/appcontrol/data/type"
#define APP_CONTROL_DATA_ITEM_ID "http://tizen.org/appcontrol/data/item_id"
#define APP_CONTROL_DATA_VCS_PATH "http://tizen.org/appcontrol/data/vcs_path"
#define APP_CONTROL_TOTAL_SIZE "http://tizen.org/appcontrol/data/total_size"
#define APP_CONTROL_TOTAL_COUNT "http://tizen.org/appcontrol/data/total_count"


#define APP_CONTROL_DATA_SELECTION_MODE_SINGLE "single"
#define APP_CONTROL_DATA_SELECTION_MODE_MULTIPLE "multiple"
#define APP_CONTROL_DATA_RESULT_TYPE_ITEM_ID "item_id"
#define APP_CONTROL_DATA_RESULT_TYPE_PHONE "phone"
#define APP_CONTROL_MY_PROFILE_DATA_TYPE "my_profile"

#define APP_CONTROL_DATA_RESULT_TYPE_EMAIL "email"
#define APP_CONTROL_DATA_RESULT_TYPE_VCARD "vcard"

#define APP_CONTROL_RES_CALLBACK(ClassName, method) [](app_control_h request, app_control_h reply, app_control_result_e result, void *data)\
{                                                                        \
    return static_cast<ClassName*>(data)->method(request, reply, result);\
}
namespace Msg
{
    struct AppControlUtils
    {
        /**
         * Gets a single string from given app-control handle by a key specified
         */
        static std::string getExtraData(app_control_h handle, const std::string &key);

        /**
        * Gets a string-array from given app-control handle by a key specified
        */
        static void getExtraDataArray(app_control_h handle, const std::string &key, std::list<std::string> &outArray);

        /**
         * Gets an int-array from given app-control handle by a key specified.
         */
        static void getExtraDataIntArray(app_control_h handle, const std::string &key, std::list<int> &outArray);
    };
}

#endif /* AppControlUtils_h_ */
