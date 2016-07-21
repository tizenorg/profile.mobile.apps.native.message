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

#ifndef SettingsOnOffListViewItem_h_
#define SettingsOnOffListViewItem_h_

#include "ListItem.h"

namespace Msg
{
    class ISettingsListViewItemListener;

    /**
     * @brief Generic visual representation of messages-settings list element.
     */
    class SettingsListViewItem
        : public ListItem
    {
        public:
            static ListItemStyleRef multiLineStyle;     /**< style for "multiline" genlist-item. */
            static ListItemStyleRef multiLineIconStyle; /**< style for "multiline + icon" genlist-item. */
            static ListItemStyleRef oneLineIconStyle;   /**< style for "single-line + icon" genlist-item. */

        public:
            /**
             * @brief Constructs list-item style based on a genlist-item style passed from outside.
             * @param[in] style to be applied.
             */
            SettingsListViewItem(const ListItemStyleRef &style);
            virtual ~SettingsListViewItem();

            /**
             * @brief Manages visibility of check-button at the moment when user creates it.
             * @param[in] show if true shows check-button, otherwise hides it.
             */
            void showCheckButton(bool show);

            /**
             * @brief Enables\disables check-button at moment of it's creation. Nothing happens if showCheckButton(false) was called before.
             * @param[in] disabled if true, check-button is created disabled, if false - check-button is enabled.
             */
            void disabledCheckButton(bool disabled);

            /**
             * @brief checks\unchecks check-button at moment of it's creation. Nothing happens if showCheckButton(false) was called before.
             * @param[in] state if true, check-button is created checked, if false - check-button is unchecked.
             */
            void setCheckButtonState(bool state);

            /**
             * @brief Gets state of check-button.
             * @return true if check-button is checked, otherwise false. Also returns false if no check-button was created.
             */
            bool getCheckButtonState() const;

            /**
             * @brief Switches state of check-button to opposite(checked/unchecked).
             */
            void changeCheckButtonState();

            /**
             * @brief Sets text to "elm.text" part.
             * @param[in] text a text to be set.
             */
            void setMainText(const std::string &text);

            /**
             * @brief Sets text to "elm.text.multiline" part. If instance of SettingsListViewItem have single-line style nothing happens.
             * @param[in] text a text to be set.
             */
            void setSubText(const std::string &text);

            /**
             * @brief Sets a listener to notify subscriber about list-item events.
             * @param[in] listener a listener to be notified.
             */
            void setListener(ISettingsListViewItemListener *listener);

        private:
            // ListItem:
            virtual std::string getText(ListItem &item, const char *part);
            virtual Evas_Object *getContent(ListItem &item, const char *part);

            void onCheckButtonChanged(Evas_Object *obj, void *eventInfo);

            Evas_Object *createCheckButton(Evas_Object *parent);

        private:
            std::string m_MainText;
            std::string m_SubText;
            bool m_ShowCheckButton;
            Eina_Bool m_CheckButtonState;
            bool m_DisabledCheckButton;
            ISettingsListViewItemListener *m_pListener;
    };

    /**
     * @brief Listener to send list-item events to subscribers.
     */
    class ISettingsListViewItemListener
    {
        public:
            virtual ~ISettingsListViewItemListener(){}

            /**
             * @brief raised when state of check-button has been changed.
             * @param[in] item an item that owns a check-button with state modified.
             */
            virtual void onCheckButtonChanged(SettingsListViewItem &item) {};
    };
}

#endif // SettingsOnOffListViewItem_h_
