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
#include "App.h"
#include "ConvListItem.h"

namespace Msg
{
    class IConvListListener;

    class ConvList
        : public ConvListLayout
        , private IMsgStorageListener
        , private IListViewListener
        , private IConvListItemListener
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
             * @param[in] App ref. to main application
             */
            ConvList(Evas_Object *parent, App &app);
            virtual ~ConvList();

            /**
             * @brief Sets listener
             * @param[in] l IConvListListener pointer
             */
            void setListener(IConvListListener *l);

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

            /**
             * @brief Navigate to mesage
             * @param[in] msgId message id to navigate
             */
            void navigateTo(MsgId msgId);

            /**
             * @brief Deletes selected items in SelectMode
             */
            void deleteSelectedItems();

            /**
             * @brief Get message count
             * @return message count
             */
            int getMessageCount() const;

        private:
            void create(Evas_Object *parent);
            Evas_Object *createSelectAll(Evas_Object *parent);
            Evas_Object *createList(Evas_Object *parent);
            void fill();
            void selectListItems(bool state);
            bool isAllListItemSelected() const;
            ConvListItem *getItem(MsgId msgId) const;

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem);
            virtual void onListItemChecked(ListItem &listItem);

            // IMsgStorageListener:
            virtual void onMsgStorageUpdate(const MsgIdList &msgIdList);
            virtual void onMsgStorageInsert(const MsgIdList &msgIdList);
            virtual void onMsgStorageDelete(const MsgIdList &msgIdList);
            virtual void onMsgStorageContact(const MsgIdList &msgIdList) {};

            // IConvListItemListener:
            virtual void onEditDraftMsg(ConvListItem &item);
            virtual void onForwardMsg(ConvListItem &item);

            // SelectAll callback:
            void onSelectAllChanged(Evas_Object *obj, void *eventInfo);

        private:
            Mode m_Mode;
            MsgEngine &m_MsgEngine;
            ThreadId m_ThreadId;
            ConvSelectAll *m_pSelectAll;
            ListView *m_pList;
            IConvListListener *m_pListner;
            App &m_App;
    };

    class IConvListListener
    {
        public:
            virtual ~IConvListListener() {};

            /**
             * @brief called when all messages has been deleted from current thread
             */
            virtual void onAllItemsDeleted(ConvList &list) {};
            virtual void onEditDraftMsg(MsgId id) {};
            virtual void onForwardMsg(MsgId id) {};
    };
}

#endif /* ConvList_h_ */
