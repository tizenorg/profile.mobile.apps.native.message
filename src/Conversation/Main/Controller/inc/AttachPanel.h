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

#ifndef AttachPanel_h_
#define AttachPanel_h_

#include "App.h"

#include <string>
#include <list>
#include <attach_panel.h>

namespace Msg
{
    class IAttachPanelListener;

    class AttachPanel
    {
        public:
            typedef std::list<std::string> FileList;

        public:
            AttachPanel(App &app);
            ~AttachPanel();

            void setListener(IAttachPanelListener *l);
            void show(bool value);
            bool isVisible() const;

        private:
            AttachPanel(const AttachPanel&) = delete;
            AttachPanel& operator=(AttachPanel&) = delete;

            attach_panel_h getAttachPanel();

            static void result_cb(attach_panel_h attachPanel, attach_panel_content_category_e contentCategory,
                        app_control_h result, app_control_result_e resultCode, void *userData);
        private:
            App &m_App;
            attach_panel_h m_Handle;
            IAttachPanelListener *m_pListener;
    };

    class IAttachPanelListener
    {
        public:
            virtual ~IAttachPanelListener() {}
            virtual void onFileSelected(AttachPanel &panel, const AttachPanel::FileList &file) {};
    };
}

#endif /* AttachPanel_h_ */
