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
#include "FileUtils.h"
#include "TimeUtils.h"
#include "SaveAttachmentsPopup.h"
#include "TextDecorator.h"
#include "MediaType.h"
#include "MediaUtils.h"
#include "TimeUtils.h"
#include "FileViewer.h"

#include <notification_status.h>

using namespace Msg;
namespace
{

    bool isContentFind(const std::string &str, const std::string &searchWord)
    {
        if(str.empty() || searchWord.empty())
            return false;

        char *found = strcasestr((char*)str.c_str(), (char*)searchWord.c_str());
        return found;
    }
}

ConvListItem::ConvListItem(const MsgConversationItem &item,
                           App &app,
                           FileViewer &fileViewer,
                           WorkingDirRef workingDir,
                           const std::string &searchWord,
                           const ThumbnailMaker::ThumbId &thumbId)
    : ConvListViewItem(getConvItemType(item))
    , m_pListener(nullptr)
    , m_App(app)
    , m_WorkingDir(workingDir)
    , m_FileViewer(fileViewer)
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

void ConvListItem::updateTime()
{
    m_TimeStr.clear();
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

void ConvListItem::addVideoItem(const MsgConvMedia &media)
{
    const std::string thumbFileName = "thumbnail.jpeg";
    std::string thumbFilePath =  m_WorkingDir->genUniqueFilePath(thumbFileName);

    if(!thumbFilePath.empty())
    {
        std::string path = media.getPath();
        if(MediaUtils::getVideoFrame(path, thumbFilePath))
            m_BubbleEntity.addItem(BubbleEntity::VideoItem, thumbFilePath, path);
    }
}

void ConvListItem::addAudioItem(const MsgConvMedia &media)
{
    std::string dsipName = media.getName();
    std::string path = media.getPath();
    if(dsipName.empty())
        dsipName = FileUtils::getFileName(path);
    m_BubbleEntity.addItem(BubbleEntity::AudioItem, dsipName, path);
}

void ConvListItem::addAttachedFileItem(const MsgConvMedia &media)
{
    std::string dsipName = media.getName();
    std::string path = media.getPath();
    if(dsipName.empty())
        dsipName = FileUtils::getFileName(path);
    m_BubbleEntity.addItem(BubbleEntity::AttachedFileItem, dsipName, path);
}

void ConvListItem::addTextItem(const MsgConvMedia &media, const std::string &searchWord)
{
    // TODO: How to detect text attachment and content(text) of MMS ?
    std::string text = FileUtils::readTextFile(media.getPath());
    if(isContentFind(utf8ToMarkup(text), utf8ToMarkup(searchWord)))
        showSearch();

    // It may be required after update the UI document
    // std::string highlightedText = TextDecorator::highlightKeyword(utf8ToMarkup(text), utf8ToMarkup(searchWord));
    m_BubbleEntity.addItem(BubbleEntity::TextItem, text);
}

void ConvListItem::addImageItem(const MsgConvMedia &media)
{
    // TODO: msg service corrupts thumbnail's metadata, so it lost rotation. Use getPath instead getThumbPath until fix
    m_BubbleEntity.addItem(BubbleEntity::ImageItem, media.getPath(), media.getPath());
}

void ConvListItem::prepareBubble(const MsgConversationItem &item, const std::string &searchWord)
{
    if(!MsgUtils::isMms(m_Type))
    {
        std::string textItem = item.getText();
        if(isContentFind(utf8ToMarkup(textItem), utf8ToMarkup(searchWord)))
            showSearch();

        // It may be after to update the UI document
        // std::string highlightedText = TextDecorator::highlightKeyword(utf8ToMarkup(item.getText()), utf8ToMarkup(searchWord));
        m_BubbleEntity.addItem(BubbleEntity::TextItem, textItem);
    }
    else if(m_Type == Message::MT_MMS_Noti)
    {
        std::string text = MessageDetailContent::getMmsNotiConvListItemContent(m_App, m_MsgId);
        m_BubbleEntity.addItem(BubbleEntity::TextItem, text);
        m_BubbleEntity.addItem(BubbleEntity::DownloadButtonItem);
    }
    else
    {
        const MsgConvMediaList &list = item.getMediaList();
        for(int i = 0; i < list.getLength(); i++)
        {
            const MsgConvMedia &media = list.at(i);
            MediaTypeData mediaType = getMediaType(media.getPath());

            switch(mediaType.type)
            {
                case MsgMedia::TextType:
                    addTextItem(media, searchWord);
                    break;
                case MsgMedia::ImageType:
                    addImageItem(media);
                    break;
                case MsgMedia::AudioType:
                    addAudioItem(media);
                    break;
                case MsgMedia::VideoType:
                    addVideoItem(media);
                    break;
                default:
                    if(mediaType.mime != "application/smil")
                        addAttachedFileItem(media);
                    break;
            }
        }
    }
}

Evas_Object *ConvListItem::getBubbleContent()
{
    BubbleView *bubble = new BubbleView(*getOwner());
    bubble->fill(m_BubbleEntity);
    bubble->setListener(this);
    return *bubble;
}

Evas_Object *ConvListItem::getThumbnail()
{
    return m_App.getThumbnailMaker().getThumbById(*getOwner(), m_ThumbId);
}

Evas_Object *ConvListItem::getProgress()
{
    return m_NetworkStatus == Message::NS_Send_Pending ||
           m_NetworkStatus == Message::NS_Sending ||
           m_NetworkStatus == Message::NS_Retrieving ? createProgress() : nullptr;
}

std::string ConvListItem::getTime()
{
    if(m_TimeStr.empty())
        m_TimeStr = TimeUtils::makeBubbleTimeString(m_Time);
    return m_TimeStr;
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
    if(m_Type == Message::MT_MMS_Noti)
        ctxPopup.appendItem(msg("IDS_MSG_BUTTON_DOWNLOAD_ABB3"), nullptr, CTXPOPUP_ITEM_PRESSED_CB(ConvListItem, onDownloadItemPressed), this);

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

    if(m_NetworkStatus != Message::NS_Sending && !msgText.empty() && m_Type == Message::MT_SMS)
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
    popup.addButton(msgt("IDS_MSG_BUTTON_DELETE_ABB4"), Popup::OkButtonId, POPUP_BUTTON_CB(ConvListItem, onDeleteButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_DELETE"));
    popup.setContent(msgt("IDS_MSG_POP_1_MESSAGE_WILL_BE_DELETED"));
    popup.show();
}

void ConvListItem::onDownloadItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_App.getMsgEngine().getTransport().retrieveMessage(m_MsgId);
}

void ConvListItem::onDownloadButtonClicked()
{
    MSG_LOG("");
    m_App.getMsgEngine().getTransport().retrieveMessage(m_MsgId);
}

void ConvListItem::onItemClicked(BubbleEntity::Item &item)
{
    MSG_LOG("");
    m_FileViewer.launchWithCopy(item.value2);
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
    item.getParent().destroy();
    MessageRef msg = m_App.getMsgEngine().getStorage().getMessage(m_MsgId);
    if(msg)
    {
        msg->setMessageStorageType(Message::MS_Sim);
        m_App.getMsgEngine().getStorage().saveMessage(*msg, false);
    }

    notification_status_message_post(msg("IDS_MSGC_POP_COPIED_TO_SIM_CARD").cStr());
}

void ConvListItem::onViewDetailsItemPressed(ContextPopupItem &item)
{
    MSG_LOG("");
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(ConvListItem, onCancelButtonClicked), this);
    popup.setTitle(msgt("IDS_MSGF_HEADER_MESSAGE_DETAILS"));
    popup.setContent(MessageDetailContent::getMsgDetailsPopupContent(m_App, m_MsgId));
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
