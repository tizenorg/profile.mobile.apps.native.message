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

#include "MsgComposerPrivate.h"
#include "MessageSMSPrivate.h"
#include "MessageMmsPrivate.h"
#include "MsgUtilsPrivate.h"
#include "MsgDefPrivate.h"

#include <msg.h>

using namespace Msg;


MsgComposerPrivate::MsgComposerPrivate()
{

}

MsgComposerPrivate::~MsgComposerPrivate()
{

}

MessageSMSRef MsgComposerPrivate::createSms()
{
    MessageSMSRef smsRef;
    msg_struct_t msgInfo = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);
    if(msgInfo)
    {
        msg_set_int_value(msgInfo, MSG_MESSAGE_TYPE_INT, MSG_TYPE_SMS);

        MessageSMSPrivate *sms = new MessageSMSPrivate(true, msgInfo);
        smsRef.reset(sms);
    }
    return smsRef;
}

MessageMmsRef MsgComposerPrivate::createMms()
{
    MessageMmsRef mmsRef;
    msg_struct_t msgInfo = msg_create_struct(MSG_STRUCT_MESSAGE_INFO);
    if(msgInfo)
    {
        msg_set_int_value(msgInfo, MSG_MESSAGE_TYPE_INT, MSG_TYPE_MMS);
        msg_struct_t mmsStruct = msg_create_struct(MSG_STRUCT_MMS);

        setSmilHeader(mmsStruct, false); // BodyView support only Image after Text layout order isTextTop = false

        MessageMmsPrivate *mms = new MessageMmsPrivate(true, msgInfo, mmsStruct);
        mmsRef.reset(mms);
    }
    return mmsRef;
}

void MsgComposerPrivate::setSmilHeader(msg_struct_t mms, bool isTextTop)
{
    msg_struct_t textRegion = NULL;
    msg_struct_t imageRegion = NULL;

    /************  make rootlayout ***************/
    msg_set_int_value(mms, MSG_MMS_ROOTLAYOUT_WIDTH_INT, smilScreenWidth);
    msg_set_int_value(mms, MSG_MMS_ROOTLAYOUT_HEIGHT_INT, smilScreenHeight);
    msg_set_int_value(mms, MSG_MMS_ROOTLAYOUT_BGCOLOR_INT, smilDefaultBgColor);

    /************ make region ********************/
    /* 1. add text region */
    msg_list_add_item(mms, MSG_STRUCT_MMS_REGION, &textRegion);
    MsgUtilsPrivate::setStr(textRegion, MSG_MMS_REGION_ID_STR, textRegionId);
    msg_set_int_value(textRegion, MSG_MMS_REGION_LENGTH_LEFT_INT, smilPosLeft);
    msg_set_bool_value(textRegion, MSG_MMS_REGION_LENGTH_LEFT_PERCENT_BOOL, true);

    int textVertPos = isTextTop ? smilPosTop : smilPosBottom;
    msg_set_int_value(textRegion, MSG_MMS_REGION_LENGTH_TOP_INT, textVertPos);

    msg_set_bool_value(textRegion, MSG_MMS_REGION_LENGTH_TOP_PERCENT_BOOL, true);

    msg_set_int_value(textRegion, MSG_MMS_REGION_LENGTH_WIDTH_INT, smilRegionWidth);
    msg_set_bool_value(textRegion, MSG_MMS_REGION_LENGTH_WIDTH_PERCENT_BOOL, true);

    msg_set_int_value(textRegion, MSG_MMS_REGION_LENGTH_HEIGHT_INT, smilRegionHeight);
    msg_set_bool_value(textRegion, MSG_MMS_REGION_LENGTH_HEIGHT_PERCENT_BOOL, true);

    msg_set_int_value(textRegion, MSG_MMS_REGION_BGCOLOR_INT, smilDefaultBgColor);

    /* 2. image(video) region */
    msg_list_add_item(mms, MSG_STRUCT_MMS_REGION, &imageRegion);
    MsgUtilsPrivate::setStr(imageRegion, MSG_MMS_REGION_ID_STR, imageRegionId);
    msg_set_int_value(imageRegion, MSG_MMS_REGION_LENGTH_LEFT_INT, smilPosLeft);
    msg_set_bool_value(imageRegion, MSG_MMS_REGION_LENGTH_LEFT_PERCENT_BOOL, true);

    int imgVertPos = isTextTop ? smilPosBottom : smilPosTop;
    msg_set_int_value(imageRegion, MSG_MMS_REGION_LENGTH_TOP_INT, imgVertPos);

    msg_set_bool_value(imageRegion, MSG_MMS_REGION_LENGTH_TOP_PERCENT_BOOL, true);
    msg_set_int_value(imageRegion, MSG_MMS_REGION_LENGTH_WIDTH_INT, smilRegionWidth);
    msg_set_bool_value(imageRegion, MSG_MMS_REGION_LENGTH_WIDTH_PERCENT_BOOL, true);
    msg_set_int_value(imageRegion, MSG_MMS_REGION_LENGTH_HEIGHT_INT, smilRegionHeight);
    msg_set_bool_value(imageRegion, MSG_MMS_REGION_LENGTH_HEIGHT_PERCENT_BOOL, true);
    msg_set_int_value(imageRegion, MSG_MMS_REGION_BGCOLOR_INT, smilDefaultBgColor);
}
