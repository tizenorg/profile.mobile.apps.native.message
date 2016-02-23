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

#include "MsgAttachmentPrivate.h"
#include "MsgUtilsPrivate.h"
#include "FileUtils.h"
#include "Logger.h"

using namespace Msg;

MsgAttachmentPrivate::MsgAttachmentPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , MsgAttachment()
{
}

MsgAttachmentPrivate::~MsgAttachmentPrivate()
{

}

std::string MsgAttachmentPrivate::getFilePath() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_ATTACH_FILEPATH_STR, MSG_FILEPATH_LEN_MAX);
}

std::string MsgAttachmentPrivate::getFileName() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_ATTACH_FILENAME_STR, MSG_FILENAME_LEN_MAX);
}

int MsgAttachmentPrivate::getFileSize() const
{
    int size = 0;
    msg_get_int_value(m_MsgStruct, MSG_MMS_ATTACH_FILESIZE_INT, &size);

    if(size == 0)
    {
        MSG_LOG_WARN("File ", getFilePath(), " size = 0, trying to get real file size");
        size = (int)FileUtils::getFileSize(getFilePath());
        MSG_LOG("Real file size = ", size);
    }

    return size;
}

std::string MsgAttachmentPrivate::getMime() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_ATTACH_CONTENT_TYPE_STR, MAX_MIME_TYPE_LEN);
}

void MsgAttachmentPrivate::setFilePath(const std::string &path)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_ATTACH_FILEPATH_STR, path);
}

void MsgAttachmentPrivate::setFileName(const std::string &fileName)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_ATTACH_FILENAME_STR, fileName);
}

void MsgAttachmentPrivate::setFileSize(int size) const
{
    msg_set_int_value(m_MsgStruct, MSG_MMS_ATTACH_FILESIZE_INT, size);
}

void MsgAttachmentPrivate::setMime(const std::string &mime)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_ATTACH_CONTENT_TYPE_STR, mime);
}



