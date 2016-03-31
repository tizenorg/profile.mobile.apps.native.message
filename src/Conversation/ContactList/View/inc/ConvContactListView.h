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

#ifndef ConvContactListView_h_
#define ConvContactListView_h_

#include "ListView.h"

namespace Msg
{
    class ConvContactListView
        : public View
    {
        public:
            ConvContactListView(Evas_Object *parent);
            virtual ~ConvContactListView();

            ListView &getList();
            const ListView &getList() const;

        private:
            int getItemHeight() const;
            void recalcGeometry();
            Evas_Object *createRect(Evas_Object *parent);
            void onGeometryChanged(Evas_Object *obj, void *eventInfo);

        private:
            Evas_Object *m_pRect;
            Evas_Object *m_pBox;
            ListView *m_pList;
    };
}

#endif /* ConvContactListView_h_ */
