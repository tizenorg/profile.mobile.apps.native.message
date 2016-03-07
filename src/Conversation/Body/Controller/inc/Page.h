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

#ifndef Page_h_
#define Page_h_

#include "PageView.h"
#include "MsgEngine.h"

namespace Msg
{
    class Body;
    class Page
        : public PageView
    {
        public:


        public:
            Page(Body &parent);
            virtual ~Page();

            const MsgTextMetric &getTextMetric();
            long long getSize();
            bool isMms();

        private:
            void updateMsgMetricIfNeeded();

        private:
            MsgTextMetric m_MsgMetric;
    };
}

#endif /* Page_h_ */
