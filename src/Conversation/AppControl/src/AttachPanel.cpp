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

#include "AttachPanel.h"
#include "Logger.h"
#include "AppControlUtils.h"

#include <bundle.h>
#include <stdio.h>

using namespace Msg;

namespace
{
    const char *appControlTotalSize = "http://tizen.org/appcontrol/data/total_size";
    const char *appControlTotalCount = "http://tizen.org/appcontrol/data/total_count";
}

AttachPanel::AttachPanel(App &app)
    : m_App(app)
    , m_Handle()
    , m_pListener(nullptr)
{

}

AttachPanel::~AttachPanel()
{
    if(m_Handle)
    {
        attach_panel_hide(m_Handle);
        attach_panel_destroy(m_Handle);
    }
}

void AttachPanel::setListener(IAttachPanelListener *l)
{
    m_pListener = l;
}

void AttachPanel::show(bool value)
{
    if(!value && !m_Handle)
        return;

    getAttachPanel();
    if(value)
        attach_panel_show(m_Handle);
    else
        attach_panel_hide(m_Handle);
}

bool AttachPanel::isVisible() const
{
    bool res = false;
    if(m_Handle)
        attach_panel_get_visibility(m_Handle, &res);
    return res;
}

attach_panel_h AttachPanel::getAttachPanel()
{
    if(m_Handle)
        return m_Handle;

    attach_panel_create(m_App.getWindow().getConformEvasObject(), &m_Handle);
    if(m_Handle)
    {
        bundle *extraData = bundle_create();

        int maxMmsSize = m_App.getMsgEngine().getSettings().getMaxMmsSize();
        MSG_LOG("Mss mms size = ", maxMmsSize);

        if(maxMmsSize > 0)
            bundle_add_str(extraData, appControlTotalSize, std::to_string(maxMmsSize).c_str());

        std::string maxAttachment = std::to_string(m_App.getMsgEngine().getSettings().getAttachmentsMaxCount());
        bundle_add_str(extraData, appControlTotalCount, maxAttachment.c_str());

        if(extraData)
        {
            attach_panel_add_content_category(m_Handle, ATTACH_PANEL_CONTENT_CATEGORY_IMAGE, extraData);
            attach_panel_add_content_category(m_Handle, ATTACH_PANEL_CONTENT_CATEGORY_CAMERA, extraData);
            attach_panel_add_content_category(m_Handle, ATTACH_PANEL_CONTENT_CATEGORY_VOICE, extraData);
            attach_panel_add_content_category(m_Handle, ATTACH_PANEL_CONTENT_CATEGORY_VIDEO, extraData);
            attach_panel_add_content_category(m_Handle, ATTACH_PANEL_CONTENT_CATEGORY_AUDIO, extraData);
            attach_panel_add_content_category(m_Handle, ATTACH_PANEL_CONTENT_CATEGORY_MYFILES, extraData);

            bundle_free(extraData);
            attach_panel_set_result_cb(m_Handle, result_cb, this);
        }
    }

    return m_Handle;
}

void AttachPanel::result_cb(attach_panel_h attachPanel, attach_panel_content_category_e contentCategory,
                            app_control_h result, app_control_result_e resultCode, void *userData)
{
    AttachPanel *self = (AttachPanel*)userData;
    if(!self->m_pListener || resultCode != APP_CONTROL_RESULT_SUCCEEDED)
        return;

    FileList fileList;
    AppControlUtils::getExtraDataArray(result, APP_CONTROL_DATA_SELECTED, fileList);

    if(!fileList.empty())
        self->m_pListener->onFileSelected(*self, fileList);

}
