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
#include <telephony_common.h>
#include <telephony_sim.h>
#include "FileUtils.h"
#include "TimeUtils.h"
#include "SaveAttachmentsPopup.h"
#include "TextDecorator.h"
#include "MediaType.h"
#include "MediaUtils.h"

using namespace Msg;

ConvListItem::ConvListItem(const MsgConversationItem &item,
                           App &app,
                           WorkingDirRef workingDir,
                           const std::string &searchWord,
                           const ThumbnailMaker::ThumbId &thumbId)
    : ConvListViewItem(getConvItemType(item))
    , m_pListener(nullptr)
    , m_App(app)
    , m_WorkingDir(workingDir)
    , m_MsgId(item.getMsgId())
    , m_IsDraft(item.isDraft())
    , m_NetworkStatus(item.getNetworkStatus())
    , m_Type(item.getType())
    , m_Time(item.getTime())
    , m_BubbleEntity()
    , m_ThumbId(thumbId)
{
    prepareBubble(item, searchWord);
}

ConvListItem::~ConvListItem()
{
}

void ConvListItem::updateStatus()
{
    MessageRef msg = m_App.getMsgEngine().getStorage().getMessage(m_MsgId);
    if(msg)
    {
        m_Time = msg->getTime();
        m_NetworkStatus = msg->getNetworkStatus();
    }

    if(m_NetworkStatus == Message::NS_Send_Fail)
        updateItemType(ConvItemType::Failed);
    else if(m_NetworkStatus == Message::NS_Send_Success || m_NetworkStatus == Message::NS_Sending)
        updateItemType(ConvItemType::Sent);
    else if(m_NetworkStatus == Message::NS_Not_Send)
        updateItemType(ConvItemType::Draft);
    else if(m_NetworkStatus == Message::NS_Received)
        updateItemType(ConvItemType::Received);
    update();
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

void ConvListItem::addVideoItem(const std::string &path)
{
    const std::string thumbFileName = "thumbnail.jpeg";
    std::string thumbFilePath =  m_WorkingDir->genUniqueFilePath(thumbFileName);

    if(!thumbFilePath.empty())
    {
        if(MediaUtils::getVideoFrame(path, thumbFilePath))
            m_BubbleEntity.addItem(BubbleEntity::VideoItem, thumbFilePath);
    }
}

void ConvListItem::prepareBubble(const MsgConversationItem &item, const std::string &searchWord)
{
    if(m_Type == Message::MT_SMS)
    {
        std::string highlightedText = TextDecorator::highlightKeyword(item.getText(), searchWord);
        m_BubbleEntity.addItem(BubbleEntity::TextItem, utf8ToMarkup(highlightedText.c_str()));
    }
    else
    {
        const MsgConvMediaList &list = item.getMediaList();
        for(int i = 0; i < list.getLength(); i++)
        {
            const MsgConvMedia &media = list.at(i);

            std::string mime = media.getMime();
            MediaTypeData mediaType = getMediaType(media.getPath());

            if(mediaType.type == MsgMedia::ImageType)
            {
                // TODO: msg service corrupts thumbnail's metadata, so it lost rotation. Use getPath instead getThumbPath until fix
                m_BubbleEntity.addItem(BubbleEntity::ImageItem, media.getPath());
            }
            else if(mediaType.type == MsgMedia::VideoType)
            {
                addVideoItem(media.getPath());
            }
            else if(mime == "text/plain")
            {
                // TODO: How to detect text attachment and content(text) of MMS ?
                std::string text = FileUtils::readTextFile(media.getPath());
                std::string highlightedText = TextDecorator::highlightKeyword(std::move(text), searchWord);
                m_BubbleEntity.addItem(BubbleEntity::TextItem, utf8ToMarkup(highlightedText.c_str()));
            }
            else if(mime != "application/smil")
            {
                m_BubbleEntity.addItem(BubbleEntity::TextItem, media.getName());
            }
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
    return m_App.getThumbnailMaker().getThumbById(*getOwner(), m_ThumbId);
}

Evas_Object *ConvListItem::getProgress()
{
    return m_NetworkStatus == Message::NS_Send_Pending ||
           m_NetworkStatus == Message::NS_Sending  ? createProgress() : nullptr;
}

std::string ConvListItem::getTime()
{
    return TimeUtils::makeBubbleTimeString(m_Time);
}

MsgId ConvListItem::getMsgId() const
{
    return m_MsgId;
}

time_t ConvListItem::getRawTime() const
{
    return m_Time;
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

    std::string msgText = getAllMsgText();

    if(m_NetworkStatus == Message::NS_Send_Fail)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_RESEND"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onResendItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onDeleteItemPressed), this);

    if(m_Type == Message::MT_MMS)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_VIEW_AS_SLIDESHOW_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onSlideShowItemPressed), this);

    if(!msgText.empty())
        ctxPopup.appendItem(msg("IDS_MSG_OPT_COPY_TEXT"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onCopyTextItemPressed), this);

    ctxPopup.appendItem(msg("IDS_MSGF_OPT_FORWARD"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onForwardItemPressed), this);

    if(m_NetworkStatus == Message::NS_Send_Fail)
        ctxPopup.appendItem(msg("IDS_MSG_OPT_EDIT"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onEditItemPressed), this);

    if(m_Type == Message::MT_MMS)
    {
        MessageMmsRef mms = std::dynamic_pointer_cast<MessageMms>(m_App.getMsgEngine().getStorage().getMessage(m_MsgId));
        if(mms && (!mms->getAttachmentList().isEmpty() || mms->getMediaCount() > 0))
            ctxPopup.appendItem(msg("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onSaveAttachmentsItemPressed), this);
    }

    if(m_NetworkStatus != Message::NS_Sending && !msgText.empty())
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
    popup.addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(ConvListItem, onCancelButtonClicked), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_RESEND_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(ConvListItem, onFailedResendButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_FAILED_TO_SEND_MESSAGE_ABB"));
    popup.setContent(msgt("IDS_MSG_POP_THIS_MESSAGE_WILL_BE_RESENT"));
    popup.show();
}

void ConvListItem::onDeleteItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(ConvListItem, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(ConvListItem, onCancelButtonClicked), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_REMOVE_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(ConvListItem, onDeleteButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_DELETE"));
    popup.setContent(msgt("IDS_MSG_POP_1_MESSAGE_WILL_BE_DELETED"));
    popup.show();
}

void ConvListItem::onCopyTextItemPressed(ContextPopupItem &item)
{
    item.getParent().destroy();
    std::string text = getAllMsgText();
    if(!text.empty())
        elm_cnp_selection_set(*getOwner(), ELM_SEL_TYPE_CLIPBOARD, ELM_SEL_FORMAT_TEXT, text.c_str(), text.length());
}

std::string ConvListItem::getAllMsgText() const
{
    // Warning: slow function
    MessageRef msg = m_App.getMsgEngine().getStorage().getMessage(m_MsgId);
    return msg ? msg->getText() : "";
}

void ConvListItem::onForwardItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    if(m_pListener)
        m_pListener->onForwardMsg(*this);
}

void ConvListItem::onResendItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    showFailedToSendPopup();
}

void ConvListItem::onSlideShowItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    if(m_pListener)
        m_pListener->onSlideShow(*this);
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
    MessageMmsRef mms = std::dynamic_pointer_cast<MessageMms>(m_App.getMsgEngine().getStorage().getMessage(m_MsgId));
    if(mms)
    {
        SaveAttachmentsPopup *popup = new SaveAttachmentsPopup(m_App, *mms);
        m_App.getPopupManager().reset(*popup);
        popup->show();
    }
}

void ConvListItem::onCopyToSimCardItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
}

void ConvListItem::onViewDetailsItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(ConvListItem, onCancelButtonClicked), this);
    popup.setTitle(msgt("IDS_MSGF_HEADER_MESSAGE_DETAILS"));
    popup.setContent(MessageDetailContent::getMsgDetailContent(m_App, m_MsgId));
    popup.show();
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

void ConvListItem::onCancelButtonClicked(Popup &popup, int buttonId)
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

void ConvListItem::onDeleteButtonClicked(Popup &popup, int buttonId)
{
    m_App.getMsgEngine().getStorage().deleteMessage(getMsgId());
    popup.destroy();
}

void ConvListItem::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    MSG_LOG("");
}
