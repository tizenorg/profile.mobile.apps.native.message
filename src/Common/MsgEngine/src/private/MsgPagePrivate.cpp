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

#include "MsgPagePrivate.h"

using namespace Msg;

MsgPagePrivate::MsgPagePrivate(bool release, msg_struct_t msgStruct)
    : MsgStructPrivate(release, msgStruct)
    , m_MediaList(false)
    , m_Media(false)
{

}

MsgPagePrivate::~MsgPagePrivate()
{
}

MsgMediaListHandlePrivate &MsgPagePrivate::getMediaList()
{
    msg_list_handle_t mediaList = nullptr;
    msg_get_list_handle(m_MsgStruct, MSG_MMS_PAGE_MEDIA_LIST_HND, (void **)&mediaList);
    m_MediaList.set(mediaList);
    return m_MediaList;
}

MsgMediaPrivate &MsgPagePrivate::addMedia()
{
    msg_struct_t media = nullptr;
    msg_list_add_item(m_MsgStruct, MSG_STRUCT_MMS_MEDIA, &media);
    m_Media.set(media);
    return m_Media;
}

void MsgPagePrivate::setPageDuration(int duration)
{
    msg_set_int_value(m_MsgStruct, MSG_MMS_PAGE_PAGE_DURATION_INT, duration);
}

int MsgPagePrivate::getPageDuration() const
{
    int duration = 0;
    msg_get_int_value(m_MsgStruct, MSG_MMS_PAGE_PAGE_DURATION_INT, &duration);
    return duration;
}

