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

#ifndef ThreadList_h_
#define ThreadList_h_

#include "ListView.h"
#include "MsgStorage.h"
#include "ContactManager.h"
#include "SystemSettingsManager.h"
#include "MsgTypes.h"
#include <set>

namespace Msg
{
    class IThreadListListener;
    class SelectAllListItem;
    class ThreadListItem;
    class App;

    class ThreadList
        : public ListView
        , private IMsgStorageListener
        , private IListViewListener
        , private IContactManagerListener
        , private ISystemSettingsManager
    {
        public:
            ThreadList(Evas_Object *parent, App &app);
            virtual ~ThreadList();

            void setListener(IThreadListListener *l);
            void setDeleteMode(bool value);
            bool isDeleteModeEnabled() const;
            void deleteSelectedItems();
            int getThreadsCheckedCount() const;

        private:
            // IMsgStorageListener:
            virtual void onMsgStorageUpdate(const MsgIdList &msgIdList);
            virtual void onMsgStorageInsert(const MsgIdList &msgIdList);
            virtual void onMsgStorageDelete(const MsgIdList &msgIdList);

            // IContactManagerListener:
            virtual void onContactChanged();

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem);
            virtual void onListItemChecked(ListItem &listItem);

            // ISystemSettingsManager:
            virtual void onTimeFormatChanged();

        private:
            void showSelectAllItem(bool show, bool resetCheck = true);
            void checkAllItems(bool check);
            void checkHandler(SelectAllListItem &item);
            void checkHandler(ThreadListItem &item);
            void fillList();
            void deleteItems();
            void updateItems(const MsgIdList &idList);
            void updateItems();
            void insertItem(const MsgThreadItem &msgThreadItem);
            void insertItem(ThreadId id);
            bool isAllThreadListItemChecked() const;
            void updateSelectAllItem();
            std::set<ThreadId> getThreadIdSet(const MsgIdList &idList);
            static int cmpFunc(const ListItem &item1, const ListItem &item2);

        private:
            IThreadListListener *m_pListener;
            App &m_App;
            bool m_DeleteMode;
    };

    class IThreadListListener
    {
        public:
            virtual ~IThreadListListener() {}
            virtual void onListItemSelected(ThreadId id) {};
            virtual void onThreadListChanged() {}
            virtual void onThreadListItemChecked() {}
    };
}

#endif // ThreadList_h_
