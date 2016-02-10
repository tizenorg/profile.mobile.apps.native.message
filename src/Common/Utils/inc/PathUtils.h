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

#ifndef PathUtils_h_
#define PathUtils_h_

#include <string>
#include "Resource.h"

namespace Msg
{
    class PathUtils
    {
        public:
            /**
             * @brief Gets the absolute path to file in the application resource directory.
             * @param[in] path to file in res. dir (without slash at begin/end)
             * @return absolute path to file
             */
            static std::string getResourcePath(const std::string &filePath);

            /**
             * @brief Gets the absolute path to file in data directory which is used to store private data of the application.
             * @param[in] path to file in data dir (without slash at begin/end)
             * @return absolute path to file
             */
            static std::string getDataPath(const std::string &filePath);
    };
}

#endif /* PathUtils_h_ */
