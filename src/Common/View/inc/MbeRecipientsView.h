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

    /**
     * @brief Wraps all basic multibutton entry(mbe) operations.
     */
    class MbeRecipientsView
        : public View
    {
        public:
            MbeRecipientsView(Evas_Object *parent);
            virtual ~MbeRecipientsView();

            /**
             * @brief Appends new item into mbe.
             * @param[in] item new item to be appended.
             */
            void appendItem(MbeRecipientItem &item);

            /**
             * @brief Gets number of mbe items.
             * @return items count.
             */
            int getItemsCount() const;

            /**
             * @brief Gets mbe items collection.
             * @return mbe items.
             */
            std::vector<MbeRecipientItem*> getItems() const;

            /**
             * @brief Gets mbe's selected item.
             * @return selected item or nullptr if no item is selected.
             */
            MbeRecipientItem *getSelectedItem() const;

            /**
             * @brief Checks whether mbe is empty or not.
             * @return true if mbe is empty false otherwise.
             */
            bool isEmpty() const;

            /**
             * @brief Removes all mbe items.
             */
            void clear();

            void setListener(IMbeRecipientsListener *pListener);
        private:
            void onMbeItemClicked(Evas_Object *obj, void *eventInfo);
            void onMbeChanged(Evas_Object *obj, void *eventInfo);


        private:
            IMbeRecipientsListener *m_pListener;
            MbeRecipientItem *m_pSelectedItem;
            Ecore_Job *m_pChangedJob;
    };

    /**
     * @brief A listener class that should be implemented by all mbe event subscribers.
     */
    class IMbeRecipientsListener
    {
    public:
        virtual ~IMbeRecipientsListener() {};

        /**
         * @brief Notification about some mbe item being clicked.
         * @param[in] item clicked item.
         */
        virtual void onMbeItemClicked(MbeRecipientItem &item) {};

        /**
         * @brief Notification about mbe content being changed(Item was added or deleted).
         */
        virtual void onMbeChanged() {};
    };
}

#endif /* MbeRecipientsView_h_ */
