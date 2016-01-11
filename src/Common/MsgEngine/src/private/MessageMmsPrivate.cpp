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

#include "MessageMmsPrivate.h"
#include "MsgUtilsPrivate.h"

#include "Logger.h"

using namespace Msg;

MessageMmsPrivate::MessageMmsPrivate(bool release, msg_struct_t msgStruct, msg_struct_t mmsStruct)
    : MessagePrivate(release, msgStruct)
    , MessageMms()
    , m_MmsStruct(mmsStruct)
    , m_PageList(false)
    , m_AttachmentList(false)
    , m_Page(false)
    , m_Attachment(false)
{
    if(!m_MmsStruct)
    {
        m_MmsStruct = msg_create_struct(MSG_STRUCT_MMS);
        if(msgStruct)
        {
            msg_get_mms_struct(m_MsgStruct, m_MmsStruct);
        }
    }
}

void MessageMmsPrivate::set(msg_struct_t msgStruct)
{
    MessagePrivate::set(msgStruct);
    msg_get_mms_struct(m_MsgStruct, m_MmsStruct);
}

void MessageMmsPrivate::commit()
{
    msg_set_mms_struct(m_MsgStruct, m_MmsStruct);
}

msg_struct_t MessageMmsPrivate::getMmsStruct() const
{
    return m_MmsStruct;
}

MessageMmsPrivate::~MessageMmsPrivate()
{
    if(m_MmsStruct)
    {
        msg_release_struct(&m_MmsStruct);
    }
}

MessageMmsPrivate::Type MessageMmsPrivate::getType() const
{
    return MT_MMS;
}

void MessageMmsPrivate::setText(const std::string &text)
{
    MsgUtilsPrivate::setStr(m_MsgStruct, MSG_MESSAGE_MMS_TEXT_STR, text);
}

std::string MessageMmsPrivate::getText() const
{
    // FIXME: internal compiler error(arm only)
    int size = 0;
    msg_get_int_value(m_MsgStruct, MSG_MESSAGE_DATA_SIZE_INT, &size); // TODO: check MSG_MESSAGE_DATA_SIZE_INT == textLen ?
    return MsgUtilsPrivate::getStr(m_MsgStruct, MSG_MESSAGE_MMS_TEXT_STR, MAX_MSG_TEXT_LEN);
}

MsgPageListHandlePrivate &MessageMmsPrivate::getPageList()
{
    msg_list_handle_t list = nullptr;
    msg_get_list_handle(m_MmsStruct, MSG_MMS_PAGE_LIST_HND, (void **)&list);
    m_PageList.set(list);
    return m_PageList;
}

MsgPagePrivate &MessageMmsPrivate::addPage()
{
    msg_struct_t page = nullptr;
    msg_list_add_item(m_MmsStruct, MSG_STRUCT_MMS_PAGE, &page);
    m_Page.set(page);
    return m_Page;
}

MsgAttachmentListHandlePrivate &MessageMmsPrivate::getAttachmentList()
{
    msg_list_handle_t list = nullptr;
    msg_get_list_handle(m_MmsStruct, MSG_MMS_ATTACH_LIST_HND, (void **)&list);
    m_AttachmentList.set(list);
    return m_AttachmentList;
}

MsgAttachmentPrivate &MessageMmsPrivate::addAttachment()
{
    msg_struct_t attach = nullptr;
    msg_list_add_item(m_MmsStruct, MSG_STRUCT_MMS_ATTACH, &attach);
    m_Attachment.set(attach);
    return m_Attachment;
}

