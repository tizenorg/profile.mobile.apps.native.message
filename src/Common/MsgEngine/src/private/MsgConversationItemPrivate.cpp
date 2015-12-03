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

#include "MsgConversationItemPrivate.h"
#include "Logger.h"
#include "MsgUtilsPrivate.h"

#include <msg_storage.h>
#include <assert.h>

using namespace Msg;

MsgConversationItemPrivate::MsgConversationItemPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , m_MultipartList(false)
{

}

MsgConversationItemPrivate::~MsgConversationItemPrivate()
{

}

MsgId MsgConversationItemPrivate::getMsgId() const
{
    int id = -1;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_ID_INT, &id);
    return id;
}

ThreadId MsgConversationItemPrivate::getThreadId() const
{
    int id = -1;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_THREAD_ID_INT, &id);
    return id;
}

std::string MsgConversationItemPrivate::getText() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_CONV_MSG_TEXT_STR, MAX_MSG_TEXT_LEN);
}

std::string MsgConversationItemPrivate::getSubject() const
{
    std::string text;
    char buf[MAX_MSG_TEXT_LEN + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_SUBJECT_STR, buf, MAX_MSG_TEXT_LEN) == 0)
    {
        text.assign(buf);
    }
    return text;
}

time_t MsgConversationItemPrivate::getTime() const
{
    int time = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_DISPLAY_TIME_INT, &time);
    return time;
}

Message::Direction MsgConversationItemPrivate::getDirection() const
{
    int direction = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_DIRECTION_INT, &direction);
    return MsgUtilsPrivate::nativeToDirection(direction);
}

Message::Type MsgConversationItemPrivate::getType() const
{
    int type = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_TYPE_INT, &type);
    return MsgUtilsPrivate::nativeToMessageType(type);
}

Message::Status MsgConversationItemPrivate::getStatus() const
{
    int status = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_NETWORK_STATUS_INT, &status);
    return (Message::Status)status;
}

bool MsgConversationItemPrivate::isDraft() const
{
    int folder = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_FOLDER_ID_INT, &folder);
    return (folder == MSG_DRAFT_ID);
}

bool MsgConversationItemPrivate::isRead() const
{
    bool read = 0;
    msg_get_bool_value(m_MsgStruct, MSG_CONV_MSG_READ_BOOL, &read);
    return read;
}

int MsgConversationItemPrivate::getPagesCount() const
{
    int pages = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_PAGE_COUNT_INT, &pages);
    return pages;
}

int MsgConversationItemPrivate::getAttachCount() const
{
    int attachments = 0;
    msg_get_int_value(m_MsgStruct, MSG_CONV_MSG_ATTACH_COUNT_INT, &attachments);
    return attachments;
}

std::string MsgConversationItemPrivate::getAttachName() const
{
    std::string attachName;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_ATTACH_NAME_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        attachName.assign(buf);
    }
    return attachName;
}

std::string MsgConversationItemPrivate::getAudioName() const
{
    std::string audioName;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_AUDIO_NAME_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        audioName.assign(buf);
    }
    return audioName;
}

std::string MsgConversationItemPrivate::getImageThumbPath() const
{
    std::string imageThumb;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_IMAGE_THUMB_PATH_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        imageThumb.assign(buf);
    }
    return imageThumb;
}

std::string MsgConversationItemPrivate::getVideoThumbPath() const
{
    std::string videoThumb;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_VIDEO_THUMB_PATH_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        videoThumb.assign(buf);
    }
    return videoThumb;
}

std::string MsgConversationItemPrivate::getFirstMediaPath() const
{
    std::string mediaPath;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_CONV_MSG_1ST_MEDIA_PATH_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        mediaPath.assign(buf);
    }
    return mediaPath;
}

const MsgConvMediaListHandlePrivate &MsgConversationItemPrivate::getMediaList() const
{
    msg_list_handle_t multipartList = nullptr;
    msg_get_list_handle(m_MsgStruct, MSG_CONV_MSG_MULTIPART_HND, (void **)&multipartList);
    m_MultipartList.set(multipartList);
    return m_MultipartList;
}
