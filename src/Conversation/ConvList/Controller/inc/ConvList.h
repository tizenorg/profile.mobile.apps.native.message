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

#ifndef ConvList_h_
#define ConvList_h_

#include "ConvListLayout.h"
#include "ConvListSelectAllView.h"
#include "ListView.h"
#include "MsgEngine.h"

namespace Msg
{
    class ConvList
        : public ConvListLayout
        , private IMsgStorageListener
        , private IListViewListener
    {
        public:
            enum Mode
            {
                NormalMode,
                SelectMode
            };

        public:
            ConvList(Evas_Object *parent, MsgEngine &msgEngine, ThreadId threadId);
            virtual ~ConvList();

            void setMode(Mode mode);
            Mode getMode() const;

        private:
            void create(Evas_Object *parent);
            Evas_Object *createSelectAll(Evas_Object *parent);
            Evas_Object *createList(Evas_Object *parent);
            void fill();

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem, void *funcData);
            virtual void onListItemChecked(ListItem &listItem, void *funcData);

        private:
            Mode m_Mode;
            MsgEngine &m_MsgEngine;
            ThreadId m_ThreadId;
            ConvListSelectAllView *m_pSelectAll;
            ListView *m_pList;
    };
}

#endif /* ConvList_h_ */
