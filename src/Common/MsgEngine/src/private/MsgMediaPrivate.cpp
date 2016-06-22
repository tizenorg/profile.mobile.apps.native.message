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


#include "MsgMediaPrivate.h"
#include "MsgUtilsPrivate.h"
#include "MsgDefPrivate.h"
#include "Logger.h"
#include "MediaType.h"
#include <msg.h>

using namespace Msg;

MsgMediaPrivate::MsgMediaPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
{
}

MsgMediaPrivate::~MsgMediaPrivate()
{

}

MsgMediaPrivate::Type MsgMediaPrivate::getType() const
{
    int type = UnknownType;
    msg_get_int_value(m_MsgStruct, MSG_MMS_MEDIA_TYPE_INT, &type);
    return MsgUtilsPrivate::nativeToSmilType(type);
}

std::string MsgMediaPrivate::getFileName() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MEDIA_FILENAME_STR, MSG_FILENAME_LEN_MAX);
}

std::string MsgMediaPrivate::getFilePath() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MEDIA_FILEPATH_STR, MSG_FILEPATH_LEN_MAX);
}

std::string MsgMediaPrivate::getMime() const
{
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MMS_MEDIA_CONTENT_TYPE_STR, MAX_MIME_TYPE_LEN);
}

void MsgMediaPrivate::setType(Type type)
{
    msg_set_int_value(m_MsgStruct, MSG_MMS_MEDIA_TYPE_INT, MsgUtilsPrivate::smilTypeToNative(type));

    switch(type)
    {
        case ImageType:
        case VideoType:
        case AnimateType:
        case ImageOrVideoType:
        {
            MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_REGION_ID_STR, imageRegionId);
            break;
        }

        case AudioType:
        {
            break;
        }

        case TextType:
        {
            MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_REGION_ID_STR, textRegionId);
            break;
        }

        default:
            MSG_ASSERT(false, "Wrong SMIL type:", type);
    }
}

void MsgMediaPrivate::setMime(const std::string &mime)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_CONTENT_TYPE_STR, mime);
}

void MsgMediaPrivate::setFilePath(const std::string &path)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_FILEPATH_STR, path);
    MediaTypeData mediaData = getMsgMediaTypeByFileExt(path);
    setType(mediaData.type);
}

void MsgMediaPrivate::setFileName(const std::string &name)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_FILENAME_STR, name);
}
