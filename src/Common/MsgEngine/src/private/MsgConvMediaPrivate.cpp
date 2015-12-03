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

#include "MsgConvMediaPrivate.h"

using namespace Msg;

MsgConvMediaPrivate::MsgConvMediaPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , MsgConvMedia()
{

}

MsgConvMediaPrivate::~MsgConvMediaPrivate()
{

}

std::string MsgConvMediaPrivate::getMime() const
{
    std::string mime;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_MMS_MULTIPART_CONTENT_TYPE_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        mime.assign(buf);
    }
    return mime;
}

std::string MsgConvMediaPrivate::getName() const
{
    std::string name;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_MMS_MULTIPART_NAME_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        name.assign(buf);
    }
    return name;
}

std::string MsgConvMediaPrivate::getPath() const
{
    std::string filePath;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_MMS_MULTIPART_FILEPATH_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        filePath.assign(buf);
    }
    return filePath;
}

std::string MsgConvMediaPrivate::getThumbPath() const
{
    std::string thumbPath;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_MMS_MULTIPART_THUMBNAIL_FILEPATH_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        thumbPath.assign(buf);
    }
    return thumbPath;
}

std::string MsgConvMediaPrivate::getContentId() const
{
    std::string contentId;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_MMS_MULTIPART_CONTENT_ID_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        contentId.assign(buf);
    }
    return contentId;
}

std::string MsgConvMediaPrivate::getContentLocation() const
{
    std::string contentLoc;
    char buf[MSG_FILENAME_LEN_MAX + 1];
    if(msg_get_str_value(m_MsgStruct, MSG_MMS_MULTIPART_CONTENT_LOCATION_STR, buf, MSG_FILENAME_LEN_MAX) == 0)
    {
        contentLoc.assign(buf);
    }
    return contentLoc;
}
