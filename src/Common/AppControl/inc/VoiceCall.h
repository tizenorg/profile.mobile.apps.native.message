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

#ifndef VoiceCall_h_
#define VoiceCall_h_

#include "AppControlUtils.h"
#include <string>

namespace Msg
{
    /**
     * An utility-class aimed to make voice call passed via app-control.
     */
    class VoiceCall
    {
    public:
        VoiceCall();

        /**
         * Launches call by number
         * @param number Contact person number
         * @return true in case of success, otherwise returns false.
         */
        static bool launch(const std::string &number);

    private:
        VoiceCall(const VoiceCall&) = delete;
        VoiceCall& operator=(const VoiceCall&) = delete;
    };
}

#endif /* VoiceCall_h_ */
