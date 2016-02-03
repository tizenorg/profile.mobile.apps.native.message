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
#include "ListView.h"
#include "CallbackAssist.h"
#include "ThumbnailMaker.h"

using namespace Msg;

ConvListItem::ConvListItem(const MsgConversationItem &item, App &app)
    : ConvListViewItem(getConvItemType(item))
    , m_pListener(nullptr)
    , m_App(app)
    , m_MsgId(item.getMsgId())
    , m_MessageText(item.getText())
    , m_IsDraft(item.isDraft())
    , m_NetworkStatus(item.getNetworkStatus())
    , m_Type(item.getType())
    , m_BubbleEntity()
{
    prepareBubble(item);
}

ConvListItem::~ConvListItem()
{
}

void ConvListItem::updateStatus(MsgId id)
{
    m_MsgId = id;
    MessageRef msg = m_App.getMsgEngine().getStorage().getMessage(m_MsgId);
    if(msg)
    {
        m_NetworkStatus = msg->getNetworkStatus();
        updateProgressField();
    }
}

ConvListViewItem::ConvItemType ConvListItem::getConvItemType(const MsgConversationItem &item)
{
    ConvItemType type = ConvItemType::Sent;
    if(item.getDirection() == Message::Direction::MD_Sent)
    {
        if(item.isDraft())
            type = ConvItemType::Draft;
        else if(item.getNetworkStatus() == Message::NS_Send_Fail)
            type = ConvItemType::Failed;
    }
    else
    {
        type = ConvItemType::Received;
    }
    return type;
}

void ConvListItem::prepareBubble(const MsgConversationItem &item)
{
    if(m_Type == Message::MT_SMS)
    {
        m_BubbleEntity.addPart(BubbleEntity::TextPart, item.getText());
    }
    else
    {
        const MsgConvMediaList &list = item.getMediaList();
        for(int i = 0; i < list.getLength(); i++)
        {
            std::string mime = list.at(i).getMime();
            if(!list.at(i).getThumbPath().empty())
                //msg service corrupts thumbnail's metadata, so it lost rotation. Use getPath instead getThumbPath until fix
                m_BubbleEntity.addPart(BubbleEntity::ThumbnailPart, list.at(i).getPath());
            else if(mime == "text/plain")
                m_BubbleEntity.addPart(BubbleEntity::TextFilePart, list.at(i).getPath());
            else if(mime != "application/smil")
                m_BubbleEntity.addPart(BubbleEntity::TextPart, list.at(i).getName());
        }
    }
}

Evas_Object *ConvListItem::getBubbleContent()
{
    BubbleView *bubble = new BubbleView(*getOwner());
    bubble->fill(m_BubbleEntity);
    return *bubble;
}

Evas_Object *ConvListItem::getThumbnail()
{
    //TODO: fetch thumb from contacts
    const int thumbSize = 80;
    Evas_Object *thumb = nullptr;
    std::string thumbPath = PathUtils::getResourcePath(THUMB_CONTACT_IMG_PATH);
    thumb = ThumbnailMaker::make(*getOwner(), ThumbnailMaker::MsgType, thumbPath);
    evas_object_size_hint_min_set(thumb, thumbSize, thumbSize);
    evas_object_size_hint_max_set(thumb, thumbSize, thumbSize);
    return thumb;
}

Evas_Object *ConvListItem::getProgress()
{
    return m_NetworkStatus == Message::NS_Send_Pending ||
           m_NetworkStatus == Message::NS_Sending  ? createProgress() : nullptr;
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

void ConvListItem::setListener(IConvListItemListener *l)
{
    m_pListener = l;
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

    if(m_NetworkStatus == Message::NS_Send_Fail)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_RESEND"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onResendItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onDeleteItemPressed), this);

    if(m_Type == Message::MT_MMS)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_VIEW_AS_SLIDESHOW_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onSlideShowItemPressed), this);

    if(!m_MessageText.empty())
        ctxPopup.appendItem(msg("IDS_MSG_OPT_COPY_TEXT"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onCopyTextItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSGF_OPT_FORWARD"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onForwardItemPressed), this);

    if(m_NetworkStatus == Message::NS_Send_Fail)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_EDIT"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onEditItemPressed), this);

    if(m_Type == Message::MT_MMS)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onSaveAttachmentsItemPressed), this);

    if(m_NetworkStatus != Message::NS_Sending && !m_MessageText.empty())
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

void ConvListItem::showFailedToSendPopup()
{
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(ConvListItem, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(ConvListItem, onFailedCancelButtonClicked), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_RESEND_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(ConvListItem, onFailedResendButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_FAILED_TO_SEND_MESSAGE_ABB"));
    popup.setContent(msgt("IDS_MSG_POP_THIS_MESSAGE_WILL_BE_RESENT"));
    popup.show();
}

void ConvListItem::onDeleteItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();
    m_App.getMsgEngine().getStorage().deleteMessage(getMsgId());
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
    showFailedToSendPopup();
}

void ConvListItem::onSlideShowItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onEditItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    if(m_pListener)
        m_pListener->onEditDraftMsg(*this);
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

void ConvListItem::onEditButtonClicked(Evas_Object *obj, void *event_info)
{
    MSG_LOG("");
    if(m_pListener)
        m_pListener->onEditDraftMsg(*this);
}

void ConvListItem::onFailedButtonClicked(Evas_Object *obj, void *event_info)
{
    MSG_LOG("");
    showFailedToSendPopup();
}

void ConvListItem::onFailedCancelButtonClicked(Popup &popup, int buttonId)
{
    MSG_LOG("");
    popup.destroy();
}

void ConvListItem::onFailedResendButtonClicked(Popup &popup, int buttonId)
{
    MSG_LOG("");
    MessageRef msg = m_App.getMsgEngine().getStorage().getMessage(m_MsgId);
    if(msg)
        m_App.getMsgEngine().getTransport().sendMessage(msg);

    popup.destroy();
}

void ConvListItem::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    MSG_LOG("");
}
