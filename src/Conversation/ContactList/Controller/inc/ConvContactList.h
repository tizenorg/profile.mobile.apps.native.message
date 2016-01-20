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

#ifndef ConvContactList_h_
#define ConvContactList_h_

#include "ListView.h"
#include "ContactManager.h"
#include "ContactListItem.h"

#include <Ecore.h>

namespace Msg
{
    class IConvContactListListener;

    class ConvContactList
        : public ListView
        , private IListViewListener
    {
        public:
            ConvContactList(Evas_Object *parent, ContactManager &cm);
            virtual ~ConvContactList();

            void setListener(IConvContactListListener *l);
            void setSearchWorld(const std::string &searchWord);
            void requestSearch();

        private:
            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem);

            template<typename ContactRecord>
            void search();
            void search();
            bool onPredictSearchUpdateRequest();
            void searchInternal();

        private:
            IConvContactListListener *m_pListener;
            Ecore_Idler *m_pPredictSearchIdler;
            ContactManager &m_ContactManager;
            std::string m_SearchWord;
    };

    class IConvContactListListener
    {
        public:
            virtual ~IConvContactListListener() {}

            virtual void onContactSelected(ContactListItem &item) {};
    };
}

#endif // ConvContactList_h_
