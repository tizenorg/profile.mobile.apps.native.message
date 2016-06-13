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

#ifndef FileShare_h_
#define FileShare_h_

#include <list>
#include <string>


namespace Msg
{
    class FileShare
    {
        public:
            static bool launch(const std::list<std::string> &files);

            FileShare(const FileShare&) = delete;
            FileShare& operator=(const FileShare&) = delete;
    };
}

#endif /* FileShare_h_ */
