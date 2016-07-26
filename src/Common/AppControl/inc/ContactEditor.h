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

#ifndef ContactEditor_h_
#define ContactEditor_h_

#include <string>
#include <app_control.h>

namespace Msg
{
    class IContactEditorListener;

    /**
     *@brief An utility-class aimed to launch contacts-application with add-operation and edit-operation passed via app-control.
     */
    class ContactEditor
    {
        public:
            enum Operation
            {
                EditOp,
                CreateOp
            };

        public:
            ContactEditor();
            ~ContactEditor();

            /**
             * @brief Launches add-operation.
             * @param[in] address number or email
             * @return true in case of success, otherwise returns false.
             */
             bool launch(const std::string &address, Operation operation);

             /**
              * @brief Set listener
              * @param[in] l listener
              */
             void setListener(IContactEditorListener *l);

        private:
             ContactEditor(const ContactEditor&) = delete;
             ContactEditor& operator=(const ContactEditor&) = delete;
             bool launch(const std::string &address, const char *operation, const char *addressType);
             void onResultCb(app_control_h request, app_control_h reply, app_control_result_e result);
             void terminate();

        private:
            IContactEditorListener *m_pListener;
            app_control_h m_Handle;
    };

    class IContactEditorListener
    {
        public:
            virtual ~IContactEditorListener() {}
            virtual void onContactCreated(ContactEditor &obj) {};
            virtual void onContactChanged(ContactEditor &obj) {};
    };
}

#endif /* ContactEditor */
