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
         * Gets a single int number from given app-control handle by a key specified.
         */
        static void getExtraDataInt(app_control_h handle, const std::string &key, int &outNumber);

        /**
         * Gets an int-array from given app-control handle by a key specified.
         */
        static void getExtraDataIntArray(app_control_h handle, const std::string &key, std::list<int> &outArray);

        /**
         * Gets a single mime type string from given app-control handle.
         */
        static void getMimeType(app_control_h handle, std::string &outMime);
    };
}

#endif /* AppControlUtils_h_ */
