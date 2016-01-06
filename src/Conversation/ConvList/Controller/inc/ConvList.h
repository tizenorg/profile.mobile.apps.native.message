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
#include "ListView.h"
#include "MsgEngine.h"
#include "ConvSelectAll.h"

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
            /**
             * @brief Creates list with messages in thread
             * @param[in] parent parent Evas_Object
             * @param[in] msgEngine Message Engine
             * @param[in] threadId Thread id
             */
            ConvList(Evas_Object *parent, MsgEngine &msgEngine, ThreadId threadId);
            virtual ~ConvList();

            /**
             * @brief Sets Conversation list mode
             * @details Set SelectMode to show checkboxes. NormalMode to hide them.
             * @param[in] mode enum Mode
             */
            void setMode(Mode mode);

            /**
             * @brief Gets current Conversation list mode
             * @return Conversation list mode
             */
            Mode getMode() const;

            /**
             * @brief Sets valid thread id
             * @param[in] thread id
             */
            void setThreadId(ThreadId id);

        private:
            void create(Evas_Object *parent);
            Evas_Object *createSelectAll(Evas_Object *parent);
            Evas_Object *createList(Evas_Object *parent);
            void fill();
            void selectListItems(bool state);
            bool isAllListItemSelected() const;

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem, void *funcData);
            virtual void onListItemChecked(ListItem &listItem, void *funcData);

            // IMsgStorageListener:
            virtual void onMsgStorageUpdate(const MsgIdList &msgIdList);
            virtual void onMsgStorageInsert(const MsgIdList &msgIdList);
            virtual void onMsgStorageDelete(const MsgIdList &msgIdList);
            virtual void onMsgStorageContact(const MsgIdList &msgIdList) {};

            // SelectAll callback:
            void onSelectAllChanged(Evas_Object *obj, void *eventInfo);

        private:
            Mode m_Mode;
            MsgEngine &m_MsgEngine;
            ThreadId m_ThreadId;
            ConvSelectAll *m_pSelectAll;
            ListView *m_pList;
    };
}

#endif /* ConvList_h_ */
