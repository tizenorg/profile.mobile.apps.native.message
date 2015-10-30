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
 */


#include "MsgSettingsPrivate.h"

#include <msg_types.h>
#include <msg.h>
#include <assert.h>

namespace
{
    const char *ringtoneDefault = "default";
    const char *ringtoneSilent = "silent";
}

using namespace Msg;

MsgSettingsPrivate::MsgSettingsPrivate(msg_handle_t serviceHandle)
    : MsgSettings()
    , m_ServiceHandleImpl(serviceHandle)
    , m_GeneralOpt()
    , m_SmsSendOpt()
    , m_MmsSendOpt()
    , m_MmsRecvOpt()
{
    init();
}

MsgSettingsPrivate::~MsgSettingsPrivate()
{
    finit();
}

void MsgSettingsPrivate::init()
{
    m_GeneralOpt = msg_create_struct(MSG_STRUCT_SETTING_GENERAL_OPT);
    msg_get_general_opt(m_ServiceHandleImpl, m_GeneralOpt);

    m_SmsSendOpt = msg_create_struct(MSG_STRUCT_SETTING_SMS_SEND_OPT);
    msg_get_sms_send_opt(m_ServiceHandleImpl, m_SmsSendOpt);

    m_MmsSendOpt = msg_create_struct(MSG_STRUCT_SETTING_MMS_SEND_OPT);
    msg_get_mms_send_opt(m_ServiceHandleImpl, m_MmsSendOpt);

    m_MmsRecvOpt = msg_create_struct(MSG_STRUCT_SETTING_MMS_RECV_OPT);
    msg_get_mms_recv_opt(m_ServiceHandleImpl, m_MmsRecvOpt);
}

void MsgSettingsPrivate::finit()
{
    msg_release_struct(&m_MmsRecvOpt);
    msg_release_struct(&m_MmsSendOpt);
    msg_release_struct(&m_SmsSendOpt);
    msg_release_struct(&m_GeneralOpt);
}

int MsgSettingsPrivate::getAddressMaxLen() const
{
    return MAX_ADDRESS_VAL_LEN;
}

int MsgSettingsPrivate::getPhoneNumberMaxLen() const
{
    return MAX_PHONE_NUMBER_LEN;
}

int MsgSettingsPrivate::getAddressMaxCount() const
{
    return MAX_TO_ADDRESS_CNT;
}

int MsgSettingsPrivate::getSubjectMaxLen() const
{
    return MAX_SUBJECT_LEN;
}

int MsgSettingsPrivate::getDisplayNameMaxLen() const
{
    return MAX_DISPLAY_NAME_LEN;
}

void MsgSettingsPrivate::setAlerts(bool value)
{
    msg_set_bool_value(m_GeneralOpt, MSG_GENERAL_MSG_NOTIFICATION_BOOL, value);
    msg_set_general_opt(m_ServiceHandleImpl, m_GeneralOpt);
}

bool MsgSettingsPrivate::getAlerts() const
{
    bool res = false;
    msg_get_bool_value(m_GeneralOpt, MSG_GENERAL_MSG_NOTIFICATION_BOOL, &res);
    return res;
}

void MsgSettingsPrivate::setNotiSound(const std::string &soundPath)
{
    if (soundPath.compare(ringtoneDefault) == 0)
    {
        msg_set_int_value(m_GeneralOpt, MSG_GENERAL_RINGTONE_TYPE_INT, MSG_RINGTONE_TYPE_DEFAULT);
    }
    else if (soundPath.compare(ringtoneSilent) == 0)
    {
        msg_set_int_value(m_GeneralOpt, MSG_GENERAL_RINGTONE_TYPE_INT, MSG_RINGTONE_TYPE_SILENT);
    }
    else
    {
        msg_set_int_value(m_GeneralOpt, MSG_GENERAL_RINGTONE_TYPE_INT, MSG_RINGTONE_TYPE_USER);
        msg_set_str_value(m_GeneralOpt, MSG_GENERAL_RINGTONE_PATH_STR, soundPath.c_str(), soundPath.size());
    }
    msg_set_general_opt(m_ServiceHandleImpl, m_GeneralOpt);

    for(auto listener: m_Listeners)
    {
        listener->onNotiSoundChanged(*this);
    }
}

std::string MsgSettingsPrivate::getNotiSound() const
{
    std::string soundPath;
    int type = MSG_RINGTONE_TYPE_DEFAULT;
    msg_get_int_value(m_GeneralOpt, MSG_GENERAL_RINGTONE_TYPE_INT, &type);
    if (type == MSG_RINGTONE_TYPE_DEFAULT)
    {
        soundPath = ringtoneDefault;
    }
    else if (type == MSG_RINGTONE_TYPE_SILENT)
    {
        soundPath = ringtoneSilent;
    }
    else
    {
        char ringtone[FILENAME_MAX] = {0};
        msg_get_str_value(m_GeneralOpt, MSG_GENERAL_RINGTONE_PATH_STR, ringtone, FILENAME_MAX);
        soundPath = ringtone;
    }
    return soundPath;
}

void MsgSettingsPrivate::setVibration(bool value)
{
    msg_set_bool_value(m_GeneralOpt, MSG_GENERAL_MSG_VIBRATION_BOOL, value);
    msg_set_general_opt(m_ServiceHandleImpl, m_GeneralOpt);
}

bool MsgSettingsPrivate::getVibration() const
{
    bool res = false;
    msg_get_bool_value(m_GeneralOpt, MSG_GENERAL_MSG_VIBRATION_BOOL, &res);
    return res;
}

void MsgSettingsPrivate::setSmsDelivReport(bool value)
{
    msg_set_bool_value(m_SmsSendOpt, MSG_SMS_SENDOPT_DELIVERY_REPORT_BOOL, value);
    msg_set_sms_send_opt(m_ServiceHandleImpl, m_SmsSendOpt);
}

bool MsgSettingsPrivate::getSmsDelivReport() const
{
    bool res = false;
    msg_get_bool_value(m_SmsSendOpt, MSG_SMS_SENDOPT_DELIVERY_REPORT_BOOL, &res);
    return res;
}

void MsgSettingsPrivate::setMmsDelivReport(bool value)
{
    msg_set_bool_value(m_MmsSendOpt, MSG_MMS_SENDOPT_DELIVERY_REPORT_BOOL, value);
    msg_set_mms_send_opt(m_ServiceHandleImpl, m_MmsSendOpt);
}

bool MsgSettingsPrivate::getMmsDelivReport() const
{
    bool res = false;
    msg_get_bool_value(m_MmsSendOpt, MSG_MMS_SENDOPT_DELIVERY_REPORT_BOOL, &res);
    return res;
}

void MsgSettingsPrivate::setMmsReadReport(bool value)
{
    msg_set_bool_value(m_MmsSendOpt, MSG_MMS_SENDOPT_READ_REPLY_BOOL, value);
    msg_set_mms_send_opt(m_ServiceHandleImpl, m_MmsSendOpt);
}

bool MsgSettingsPrivate::getMmsReadReport() const
{
    bool res = false;
    msg_get_bool_value(m_MmsSendOpt, MSG_MMS_SENDOPT_READ_REPLY_BOOL, &res);
    return res;
}

void MsgSettingsPrivate::setMmsAutoRetr(bool value)
{
    int retrType= value ? MSG_HOME_AUTO_DOWNLOAD : MSG_HOME_MANUAL;
    msg_set_int_value(m_MmsRecvOpt, MSG_MMS_RECVOPT_HOME_RETRIEVE_TYPE_INT, retrType);
    msg_set_mms_recv_opt(m_ServiceHandleImpl, m_MmsRecvOpt);
}

bool MsgSettingsPrivate::getMmsAutoRetr() const
{
    bool res = false;
    int retrType = 0;
    msg_get_int_value(m_MmsRecvOpt, MSG_MMS_RECVOPT_HOME_RETRIEVE_TYPE_INT, &retrType);
    res = retrType == MSG_HOME_AUTO_DOWNLOAD;
    return res;
}

void MsgSettingsPrivate::setMmsAutoRetrRoaming(bool value)
{
    int retrType = value ? MSG_ABROAD_AUTO_DOWNLOAD : MSG_ABROAD_MANUAL;
    msg_set_int_value(m_MmsRecvOpt, MSG_MMS_RECVOPT_ABROAD_RETRIEVE_TYPE_INT, retrType);
    msg_set_mms_recv_opt(m_ServiceHandleImpl, m_MmsRecvOpt);
}

bool MsgSettingsPrivate::getMmsAutoRetrRoaming() const
{
    bool res = false;
    int retrType = 0;
    msg_get_int_value(m_MmsRecvOpt, MSG_MMS_RECVOPT_ABROAD_RETRIEVE_TYPE_INT, &retrType);
    res = retrType == MSG_ABROAD_AUTO_DOWNLOAD;
    return res;
}

