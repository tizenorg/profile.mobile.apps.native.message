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
    {
        public:
            ThreadList(Evas_Object *parent, App &app);
            virtual ~ThreadList();

            void setListener(IThreadListListener *l);
            void setDeleteMode(bool value);
            bool isDeleteModeEnabled() const;
            void deleteSelectedItems();

        private:
            // IMsgStorageListener:
            virtual void onMsgStorageChange(const MsgIdList &idList);

            // IContactManagerListener:
            virtual void onContactChanged();

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem);
            virtual void onListItemChecked(ListItem &listItem);

        private:
            void showSelectAllItem(bool show, bool resetCheck = true);
            void checkAllItems(bool check);
            void checkHandler(SelectAllListItem &item);
            void checkHandler(ThreadListItem &item);
            void updateList();
            bool isAllThreadListItemChecked() const;
            void updateSelectAllItem();

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
    };
}

#endif // ThreadList_h_
