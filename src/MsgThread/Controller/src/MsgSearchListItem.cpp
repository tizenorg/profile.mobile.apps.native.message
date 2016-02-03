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

#include "MsgSearchListItem.h"
#include "MsgEngine.h"
#include "TextDecorator.h"

using namespace Msg;

MsgSearchListItem::MsgSearchListItem(App &app, const Message &msg, const std::string &searchWord)
    : BaseThreadListItem(app)
    , m_MsgId(msg.getId())
{
    setState(NormalState, false);
    update(msg, searchWord);
}

MsgSearchListItem::~MsgSearchListItem()
{
}

MsgId MsgSearchListItem::getMsgId() const
{
    return m_MsgId;
}

void MsgSearchListItem::updateThumbnail(const Message &msg)
{
    const MsgAddressList &addressList = msg.getAddressList();
    BaseThreadListItem::updateThumbnail(addressList);
}

void MsgSearchListItem::update(const Message &msg, const std::string &searchWord)
{
    m_MsgId = msg.getId();
    m_Message = decorateMessageText(TextDecorator::highlightKeyword(msg.getText(), searchWord));

    const MsgAddressList &addressList = msg.getAddressList();
    int addrCount = addressList.getLength();
    if(addrCount > 0)
    {
        m_Name = addressList.at(0).getAddress();
        if(addrCount > 1)
            m_Name += " + " + std::to_string(addrCount - 1);
        m_Name = decorateNameText(m_Name);
    }

    updateThumbnail(msg);
    updateTime(msg.getTime());
}
