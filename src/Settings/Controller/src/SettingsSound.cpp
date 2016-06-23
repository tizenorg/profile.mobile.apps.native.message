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

#include "SettingsSound.h"
#include "MsgEngine.h"
#include "MediaUtils.h"
#include "LangUtils.h"
#include <app.h>

using namespace Msg;

namespace
{
    const char *sRingtoneKeyPath = "path";
    const char *sRingtoneValuePath = "/opt/share/settings/Alerts";
    const char *sRingtoneKeySelectType = "select_type";
    const char *sRingtoneValueSingleFile = "SINGLE_FILE";
    const char *sRingtoneKeyFileType = "file_type";
    const char *sRingtoneValueSound = "SOUND";
    const char *sRingtoneKeyMarkedMode = "marked_mode";
    const char *sRingtoneKeySilent = "silent";
    const char *sRingtoneValueSilentShow = "silent show";
    const char *sRingtoneKeyDefault = "default";
    const char *sRingtoneValueDefaultShow = "default show";
    const char *sRingtoneAppId = "setting-ringtone-efl";
    const char *sRingtoneKeyResult = "result";
    const char* sRingtoneKeyDrmType = "drm_type";
    const char* sRingtoneValueDrmAll = "DRM_ALL";
    const char* sRingtoneKeyDomain = "domain";
    const char* sRingtoneKeyTitle = "title";
}

SettingsSound::SettingsSound(MsgSettings &settingsHandle)
    : m_SettingsHandle(settingsHandle)
{
}

SettingsSound::~SettingsSound()
{
}

static void soundPickerCb(app_control_h request, app_control_h reply, app_control_result_e result, void *user_data)
{
    char *ringtonePath = nullptr;

    app_control_get_extra_data(reply, sRingtoneKeyResult, &ringtonePath);
    if (!ringtonePath)
    {
        return;
    }

    std::string title(MediaUtils::getTitle(ringtonePath));
    static_cast<MsgSettings*>(user_data)->setNotiSound(title);
    free(ringtonePath);
}

void SettingsSound::launchSoundPicker()
{
    app_control_h h;
    app_control_create(&h);
    app_control_set_launch_mode(h, APP_CONTROL_LAUNCH_MODE_GROUP);
    app_control_add_extra_data(h, sRingtoneKeyPath, sRingtoneValuePath);
    app_control_add_extra_data(h, sRingtoneKeyDomain, PACKAGE_NAME);
    app_control_add_extra_data(h, sRingtoneKeySelectType, sRingtoneValueSingleFile);
    app_control_add_extra_data(h, sRingtoneKeyFileType, sRingtoneValueSound);
    app_control_add_extra_data(h, sRingtoneKeyMarkedMode, m_SettingsHandle.getNotiSound().c_str());
    app_control_add_extra_data(h, sRingtoneKeySilent, sRingtoneValueSilentShow);
    app_control_add_extra_data(h, sRingtoneKeyDefault, sRingtoneValueDefaultShow);
    app_control_add_extra_data(h, sRingtoneKeyDrmType, sRingtoneValueDrmAll);
    app_control_add_extra_data(h, sRingtoneKeyTitle, msg("IDS_MSG_HEADER_MESSAGE_TONE_ABB").get());

    app_control_set_app_id(h, sRingtoneAppId);
    app_control_send_launch_request(h, soundPickerCb, &m_SettingsHandle);

    app_control_destroy(h);
}

