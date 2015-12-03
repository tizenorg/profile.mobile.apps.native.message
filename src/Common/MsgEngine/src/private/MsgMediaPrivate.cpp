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
#include <msg.h>

using namespace Msg;

MsgMediaPrivate::MsgMediaPrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
{
}

MsgMediaPrivate::~MsgMediaPrivate()
{

}

MsgMediaPrivate::SmilType MsgMediaPrivate::getType() const
{
    int type = SmilInvalid;
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

void MsgMediaPrivate::setType(SmilType type)
{
    msg_set_int_value(m_MsgStruct, MSG_MMS_MEDIA_TYPE_INT, MsgUtilsPrivate::smilTypeToNative(type));

    switch(type)
    {
        case SmilImage:
        case SmilVideo:
        case SmilAnimate:
        case SmilImageOrVideo:
        {
            MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_REGION_ID_STR, imageRegionId);
            break;
        }

        case SmilAudio:
        {
            break;
        }

        case SmilText:
        {
            MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_REGION_ID_STR, textRegionId);
            setMime(mimeText);;

            // Set font style:
            msg_struct_t textStyle = msg_create_struct(MSG_STRUCT_MMS_SMIL_TEXT);
            msg_set_int_value(textStyle, MSG_MMS_SMIL_TEXT_COLOR_INT, smilDefaultFontColor);
            msg_set_int_value(textStyle, MSG_MMS_SMIL_TEXT_SIZE_INT, smilDefaultFontSize);
            msg_set_int_value(textStyle, MSG_MMS_SMIL_TEXT_BOLD_BOOL, smilDefaultBold);
            msg_set_int_value(textStyle, MSG_MMS_SMIL_TEXT_UNDERLINE_BOOL, smilDefaultUnderline);
            msg_set_struct_handle(m_MsgStruct, MSG_MMS_MEDIA_SMIL_TEXT_HND, textStyle);
            msg_release_struct(&textStyle);
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
}

void MsgMediaPrivate::setFileName(const std::string &name)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MMS_MEDIA_FILENAME_STR, name);
}
