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

#ifndef MbeRecipientsView_h_
#define MbeRecipientsView_h_

#include "View.h"
#include "MbeRecipientViewItem.h"

#include <vector>

namespace Msg
{
    class IMbeRecipientsListener;

    class MbeRecipientsView
        : public View
    {
        public:
            MbeRecipientsView(Evas_Object *parent);
            virtual ~MbeRecipientsView();

            void appendItem(MbeRecipientItem &item);
            int getItemsCount() const;
            std::vector<MbeRecipientItem*> getItems() const;
            MbeRecipientItem *getSelectedItem() const;
            bool isEmpty() const;
            void clear();
            void setListener(IMbeRecipientsListener* pListener);
        private:
            void onMbeItemClicked(Evas_Object *obj, void *eventInfo);
        private:
            IMbeRecipientsListener *m_pListener;
            MbeRecipientItem *m_pSelectedItem;
    };

    class IMbeRecipientsListener
    {
    public:
        virtual ~IMbeRecipientsListener() {};
        virtual void onMbeItemClicked(MbeRecipientItem& item) = 0;
    };
}

#endif /* MbeRecipientsView_h_ */
