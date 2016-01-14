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

#ifndef AppControlCommandMain_h_
#define AppControlCommandMain_h_

#include "AppControlCommand.h"

namespace Msg
{
    class AppControlCommandMain;
    typedef std::shared_ptr<AppControlCommandMain> AppControlCommandMainRef;

    class AppControlCommandMain
        : public AppControlCommand
    {
        public:
            AppControlCommandMain(const std::string &opMsg);
            virtual ~AppControlCommandMain();

        private:
    };
}

#endif /* AppControlCommandMain_h_ */
