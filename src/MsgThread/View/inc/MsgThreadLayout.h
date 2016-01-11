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

#ifndef MsgThreadLayout_h_
#define MsgThreadLayout_h_

#include "View.h"
#include "DefaultLayout.h"

namespace Msg
{
    class MsgThreadLayout
        : public DefaultLayout
    {
        public:
            MsgThreadLayout(Evas_Object *parent);
            virtual ~MsgThreadLayout();

            void setList(Evas_Object *list);
            void setSearchList(Evas_Object *searchList);
            void setNoContent(Evas_Object *bg);
            void showNoContent(bool show);
            void showSearchList(bool show);
            void showThreadList(bool show);
        private:
            void create(Evas_Object *parent);
            void setContent(Evas_Object *obj);

        private:
            Evas_Object *m_pLayout;

    };
}

#endif // MsgThreadLayout_h_
