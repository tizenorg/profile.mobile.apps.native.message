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

#ifndef SimMsgLayout_h_
#define SimMsgLayout_h_

#include "View.h"
#include "DefaultLayout.h"

namespace Msg
{
    class SimMsgLayout
        : public DefaultLayout
    {
        public:
            SimMsgLayout(Evas_Object *parent);
            virtual ~SimMsgLayout();

            void setSimMsgList(Evas_Object *list);
            void setNoContent(Evas_Object *bg);
            void setNoContentLayoutVisible(bool show);
            void setSimMsgListVisible(bool show);
        private:
            void create(Evas_Object *parent);
            void setContent(Evas_Object *obj);

        private:
            Evas_Object *m_pLayout;

    };
}

#endif // SimMsgLayout_h_
