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

#include "ThreadSearchListItem.h"
#include "TextDecorator.h"

using namespace Msg;

ThreadSearchListItem::ThreadSearchListItem(App &app, const MsgThreadItem &item, const std::string &searchWord)
    : BaseThreadListItem(app)
{
    update(item, searchWord);
}

ThreadSearchListItem::~ThreadSearchListItem()
{

}

ThreadId ThreadSearchListItem::getThreadId() const
{
    return m_ThreadId;
}

void ThreadSearchListItem::update(const MsgThreadItem &item, const std::string &searchWord)
{
    m_ThreadId = item.getId();
    m_Message = decorateMessageText(item.getLastMessage());
    updateThumbnailAndName(item, false);
    m_Name = decorateNameText(TextDecorator::highlightKeyword(m_Name, searchWord));
    updateTime(item.getTime());
}
