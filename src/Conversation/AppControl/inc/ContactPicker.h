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

#ifndef ContactPicker_h_
#define ContactPicker_h_

#include "AppControlUtils.h"

namespace Msg
{
    class IContactPickerListener;

    /**
     * @brief An utility-class aimed to launch contacts-application with pick-operation passed via app-control.
     */
    class ContactPicker
    {
    public:
        ContactPicker();

        /**
         * @brief Sets a listener-object to be notified when pick-operation succeeds.
         */
        void setListener(IContactPickerListener *pListener);

        /**
         * @brief Launches pick-operation.
         * @param howManyToPick defines a maximum count of contacts allowed to be picked.
         * @return true in case of success, otherwise returns false.
         */
        bool launch(size_t howManyToPick);

    private:
        ContactPicker(const ContactPicker&) = delete;
        ContactPicker& operator=(const ContactPicker&) = delete;
        void onAppControlRes(app_control_h request, app_control_h reply, app_control_result_e result);

    private:
        IContactPickerListener *m_pListener;
    };

    class IContactPickerListener
    {
    public:
        virtual ~IContactPickerListener() {}
        virtual void onContactsPicked(const std::list<int> &numberIdList) = 0;
    };
}

#endif /* ContactPicker_h_ */
