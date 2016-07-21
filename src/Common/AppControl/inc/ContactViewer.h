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

#ifndef ContactViewer_h_
#define ContactViewer_h_

#include "AppControlUtils.h"
#include "ContactAddress.h"

namespace Msg
{
    /**
     *@brief An utility-class aimed to launch contacts-application with view-operation passed via app-control.
     */
    class ContactViewer
    {
        public:
            /**
             * @brief Launches view-operation.
             * @param id Contact person id or MyProfile id
             * @param ownerType PersonType or MyProfileType
             * @return true in case of success, otherwise returns false.
             */
            static bool launch(int id, ContactAddress::OwnerType ownerType);

            /**
             * @brief Launches view-operation.
             * @param address reference to ContactAddress object.
             * @return true in case of success, otherwise returns false.
             */
            static bool launch(const ContactAddress &address);

        private:
            ContactViewer(const ContactViewer&) = delete;
            ContactViewer& operator=(const ContactViewer&) = delete;
            static const char *toStr(ContactAddress::OwnerType type);
    };
}

#endif /* ContactViewer_h_ */
