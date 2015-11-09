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

#include "Conversation.h"

#include "App.h"
#include "Message.h"
#include "Logger.h"
#include "Box.h"
#include "BubbleView.h"

#include <sstream>

using namespace Msg;

void Conversation::createBubbleList(Evas_Object *parent)
{
    if(!m_pBubbleBox)
    {
        m_pBubbleBox = new Box(parent);
        m_pBubbleBox->setHorizontal(false);
        m_pBubbleBox->setHomogeneous(false);
        m_pBubbleBox->show();
    }
}

void Conversation::fillConversationList()
{
    if(m_pBubbleBox)
    {
        m_pBubbleBox->clear();

        MsgConversationListRef convList = getMsgEngine().getStorage().getConversationList(m_ThreadId);
        int convListLen = convList->getLength();

        for(int i = 0; i < convListLen; ++i)
        {
            MsgConversationItem &item = convList->at(i);
            BubbleView *bubble = new BubbleView(*m_pBubbleBox, (BubbleView::Style)item.getDirection());
            bubble->show();
            bubble->setText(item.getText());
            bubble->setTime("10:23 PM");
            m_pBubbleBox->packEnd(*bubble);
        }
        m_pBubbleBox->recalculate();
    }
}
