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

#include "MsgThread.h"
#include "App.h"
#include "Logger.h"

using namespace Msg;

Evas_Object *MsgThread::createSearchPanel(Evas_Object *parent)
{
    m_pSearchPanel = new MsgThreadSearchPanel(*m_pLayout);
    m_pSearchPanel->setListener(this);
    m_pSearchPanel->setGuideText("Search");
    m_pSearchPanel->show();
    return *m_pSearchPanel;
}

void MsgThread::setSearchMode(bool value)
{
    if(value)
    {
        m_Mode = SearchMode;
        m_pSearchPanel->clearEntry();
    }

    m_pLayout->showSearchPanel(value);
    m_pSearchPanel->setEntryFocus(value);
    getNaviBar().setVisible(!value);
}

void MsgThread::onSearchButtonClicked(MsgThreadSearchPanel &obj)
{
    MSG_LOG("");
}

void MsgThread::onEntryChanged(MsgThreadSearchPanel &obj)
{
    MSG_LOG("");
}
