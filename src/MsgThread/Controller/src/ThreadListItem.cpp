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

#include "ThreadListItem.h"
#include "ViewController.h"
#include "ListView.h"
#include "Logger.h"
#include "PathUtils.h"
#include "MsgEngine.h"
#include "App.h"
#include "MsgThreadItem.h"
#include "LangUtils.h"

#include <Elementary.h>

using namespace Msg;

ThreadListItem::ThreadListItem(const MsgThreadItem &threadItem, App &app)
    : BaseThreadListItem(app)
    , m_ThreadId()
{
    update(threadItem);
}

ThreadListItem::~ThreadListItem()
{
}

ThreadId ThreadListItem::getThreadId() const
{
    return m_ThreadId;
}

std::string ThreadListItem::getStatus()
{
    return m_Status;
}

Evas_Object *ThreadListItem::getIcon()
{
    return makeUnreadIcon(m_UnreadCount);
}

void ThreadListItem::update(const MsgThreadItem &threadItem)
{
    m_ThreadId = threadItem.getId();

    State state = NormalState;
    if(threadItem.isSending())
    {
        state = StatusState;
        m_Status = decorateSendingText(msg("IDS_MSG_BODY_SENDING_ING_M_STATUS_ABB"));
    }
    else if(threadItem.hasFailedMessage())
    {
        state = StatusState;
        m_Status = decorateFailedText(msg("IDS_MSG_BODY_FAILED_M_STATUS_ABB2"));
    }
    else if(threadItem.hasDraftMessage())
    {
        state = StatusState;
        m_Status = decorateDraftText(msg("IDS_MSG_BODY_DRAFT_M_STATUS_ABB"));
    }
    else if(int unreadCount = threadItem.getUnreadCount() > 0)
    {
        state = IconState;
        m_UnreadCount = decorateUnreadText(std::to_string(unreadCount));
    }

    setState(state, false);

    updateMessage(threadItem);
    updateThumbnailAndName(threadItem, true);
    updateTime(threadItem.getTime());
}

void ThreadListItem::update()
{
    MsgThreadItemRef msgThread = m_App.getMsgEngine().getStorage().getThread(m_ThreadId);
    if(msgThread)
        update(*msgThread);
}

