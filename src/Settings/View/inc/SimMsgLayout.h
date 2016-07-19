/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef SimMsgLayout_h_
#define SimMsgLayout_h_

#include "View.h"
#include "DefaultLayout.h"

namespace Msg
{
    /**
     * Represents a layout of "Messages on SIM" view.
     */
    class SimMsgLayout
        : public DefaultLayout
    {
        public:
            /**
             * @brief Constructs an instance of SimMsgLayout based on parent passed from outside.
             * @param[in] an object responsible for automatic destroying of SimMsgLayout instance.
             */
            SimMsgLayout(Evas_Object *parent);
            virtual ~SimMsgLayout();

            /**
             * @brief populates itself according to list passed from outside.
             * @param[in] list to be placed on layout.
             */
            void setSimMsgList(Evas_Object *list);

            /**
             * @brief acquires a layout for "No content" mode.
             * @param[in] bg a background layout that will be shown if list of messages on SIM is empty.
             */
            void setNoContent(Evas_Object *bg);

            /**
             * @brief Shows/hides "No content" layout.
             * @param[in] show if true no list is shown, but "No messages on SIM card" text, if false hides "No content" layout.
             */
            void setNoContentLayoutVisible(bool show);

            /**
             * @brief Shows/hides list of messages on SIM.
             * @param[in] show if true the list is shown, otherwise list is hidden.
             */
            void setSimMsgListVisible(bool show);
        private:
            void create(Evas_Object *parent);
            void setContent(Evas_Object *obj);

        private:
            Evas_Object *m_pLayout;

    };
}

#endif // SimMsgLayout_h_
