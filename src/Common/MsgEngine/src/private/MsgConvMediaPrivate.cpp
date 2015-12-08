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
#include "MsgUtilsPrivate.h"

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
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MULTIPART_CONTENT_TYPE_STR, MAX_MIME_TYPE_LEN);
}

std::string MsgConvMediaPrivate::getName() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MULTIPART_NAME_STR, MSG_FILENAME_LEN_MAX);
}

std::string MsgConvMediaPrivate::getPath() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MULTIPART_FILEPATH_STR, MSG_FILEPATH_LEN_MAX);
}

std::string MsgConvMediaPrivate::getThumbPath() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MULTIPART_THUMBNAIL_FILEPATH_STR, MSG_FILEPATH_LEN_MAX);
}

std::string MsgConvMediaPrivate::getContentId() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MULTIPART_CONTENT_ID_STR, MMS_MSG_ID_LEN);
}

std::string MsgConvMediaPrivate::getContentLocation() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MULTIPART_CONTENT_LOCATION_STR, MMS_LOCATION_LEN);
}
