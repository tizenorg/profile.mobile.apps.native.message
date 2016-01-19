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

#include "ConvListItem.h"
#include "MsgConversationItem.h"
#include "BubbleView.h"
#include "ListView.h"

using namespace Msg;

ConvListItem::ConvListItem(MsgConversationItem &item, App &app)
    : ConvListViewItem(item.getDirection() == Message::Direction::MD_Sent ? ConvItemType::Sent : ConvItemType::Received)
    , m_App(app)
    , m_MsgId(item.getMsgId())
    , m_MessageText(item.getText())
    , m_IsDraft(item.isDraft())
    , m_Status(item.getStatus())
    , m_Type(item.getType())
{

}

ConvListItem::~ConvListItem()
{

}

Evas_Object *ConvListItem::getBubble()
{
    //TODO: implement getting of multimedia
    return nullptr;
}

std::string ConvListItem::getText()
{
    return m_MessageText;
}

std::string ConvListItem::getTime()
{
    //TODO: convert time_t to string
    return "10:23 PM";
}

MsgId ConvListItem::getMsgId() const
{
    return m_MsgId;
}

void ConvListItem::showPopup()
{
    if(m_IsDraft)
        showDraftCtxPopup();
    else
        showMainCtxPopup();
}

void ConvListItem::showMainCtxPopup()
{
    auto &ctxPopup = m_App.getPopupManager().getCtxPopup();

    if(m_Status == Message::MS_Send_Fail)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_RESEND"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onResendItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onDeleteItemPressed), this);

    if(m_Type == Message::MT_MMS)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_VIEW_AS_SLIDESHOW_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onSlideShowItemPressed), this);

    if(!m_MessageText.empty())
        ctxPopup.appendItem(msg("IDS_MSG_OPT_COPY_TEXT"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onCopyTextItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSGF_OPT_FORWARD"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onForwardItemPressed), this);

    if(m_Status == Message::MS_Send_Fail)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_EDIT"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onEditItemPressed), this);

    if(m_Type == Message::MT_MMS)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onSaveAttachmentsItemPressed), this);

    if(m_Status != Message::MS_Sending && !m_MessageText.empty())
        ctxPopup.appendItem(msg("IDS_MSG_OPT_COPY_TO_SIM_CARD_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onCopyToSimCardItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSG_OPT_VIEW_DETAILS_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onViewDetailsItemPressed), this);

    ctxPopup.align(m_App.getWindow());
    ctxPopup.show();
}

void ConvListItem::showDraftCtxPopup()
{
    auto &ctxPopup = m_App.getPopupManager().getCtxPopup();

    ctxPopup.appendItem(msg("IDS_MSGF_OPT_EDIT_MESSAGE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onEditItemPressed), this);
    ctxPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onDeleteItemPressed), this);

    ctxPopup.align(m_App.getWindow());
    ctxPopup.show();
}

void ConvListItem::onDeleteItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onCopyTextItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onForwardItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onResendItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onSlideShowItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onEditItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onSaveAttachmentsItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onCopyToSimCardItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onViewDetailsItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

