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

#ifndef ThreadSearchList_h_
#define ThreadSearchList_h_

#include "ListView.h"
#include "App.h"
#include "MsgTypes.h"

#include <Ecore.h>

namespace Msg
{
    class IThreadSearchListListener;

    class ThreadSearchList
        : public ListView
        , private IListViewListener
    {
        public:
            ThreadSearchList(Evas_Object *parent, App &app);
            virtual ~ThreadSearchList();

            void setListener(IThreadSearchListListener *l);
            void requestSearch(const std::string &searchWord);
            void cancelSearch();
            const std::string &getSearchWord() const;

        private:
            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem);

            void search();

        private:
            App &m_App;
            Ecore_Idler *m_pIdler;
            std::string m_SearchWord;
            IThreadSearchListListener *m_pListener;
    };

    class IThreadSearchListListener
    {
        public:
            virtual ~IThreadSearchListListener() {}
            virtual void onSearchListChanged() {};
            virtual void onSearchListItemSelected(ThreadId id) {};
            virtual void onSearchListItemSelected(MsgId id, const std::string &searchWord) {};
    };
}

#endif // ThreadSearchList_h_
