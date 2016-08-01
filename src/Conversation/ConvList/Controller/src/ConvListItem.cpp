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
#include "FileViewer.h"
#include "BubbleItemContainer.h"

// Bubble items:
#include "BubbleTextEntity.h"
#include "BubbleImageEntity.h"
#include "BubbleVideoEntity.h"
#include "BubbleAudioEntity.h"
#include "BubbleDownloadButtonEntity.h"
#include "BubbleUnknownFileEntity.h"
#include "BubbleCalEventEntity.h"
#include "BubbleContactEntity.h"

#include <notification_status.h>
#include <algorithm>

using namespace Msg;

namespace
{
    bool findText(const std::string &text, const std::string &searchWord)
    {
        if(text.empty() || searchWord.empty())
            return false;

        std::string s(markupToUtf8(text));
        std::transform(s.begin(), s.end(), s.begin(), tolower);
        std::string sw(markupToUtf8(searchWord));
        std::transform(sw.begin(), sw.end(), sw.begin(), tolower);
        size_t size = s.find(sw);
        return size != std::string::npos;
    }
}

ConvListItem::ConvListItem(const MsgConversationItem &item,
                           App &app,
                           FileViewer &fileViewer,
                           WorkingDirRef workingDir,
                           const std::string &searchWord,
                           const ThumbId &thumbId)
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
    , m_ThumbId(thumbId)
{
    prepareBubble(item, searchWord);
}

ConvListItem::~ConvListItem()
{
    for(auto *entity : m_BubbleEntityList)
    {
        delete entity;
    }
    m_BubbleEntityList.clear();
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
    {
        updateItemType(ConvItemType::Failed);
        updateEntityBgType(BubbleBgViewItem::FailedStyle);
    }
    else if(m_NetworkStatus == Message::NS_Send_Success || m_NetworkStatus == Message::NS_Sending)
    {
        updateItemType(ConvItemType::Sent);
        updateEntityBgType(BubbleBgViewItem::SentStyle);
    }
    else if(m_NetworkStatus == Message::NS_Not_Send)
    {
        updateItemType(ConvItemType::Draft);
        updateEntityBgType(BubbleBgViewItem::DraftStyle);
    }
    else if(m_NetworkStatus == Message::NS_Received)
    {
        updateItemType(ConvItemType::Received);
        updateEntityBgType(BubbleBgViewItem::ReceivedStyle);
    }

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

BubbleBgViewItem::BgType ConvListItem::getBubbleBgType(const MsgConversationItem &item)
{
    BubbleBgViewItem::BgType type = BubbleBgViewItem::SentStyle;
    if(item.getDirection() == Message::Direction::MD_Sent)
    {
        if(item.isDraft())
            type = BubbleBgViewItem::DraftStyle;
        else if(item.getNetworkStatus() == Message::NS_Send_Fail)
            type = BubbleBgViewItem::FailedStyle;
    }
    else
    {
        type = BubbleBgViewItem::ReceivedStyle;
    }
    return type;
}


BubbleEntity *ConvListItem::createTextEntity(BubbleBgViewItem::BgType bgType, Message::Direction direction, const MsgConvMedia &media, const std::string &searchWord)
{
    std::string text = FileUtils::readTextFile(media.getPath());

    if(findText(text, searchWord))
        showSearch();

    return new BubbleTextEntity(bgType, direction, utf8ToMarkup(text));
}

BubbleEntity *ConvListItem::createTextEntity(BubbleBgViewItem::BgType bgType, Message::Direction direction, std::string text, bool markup, const std::string &searchWord)
{
    if(findText(text, searchWord))
        showSearch();

    std::string resText = markup ? utf8ToMarkup(text) : std::move(text);

    return new BubbleTextEntity(bgType, direction, resText);
}

void ConvListItem::addEntity(BubbleEntity *entity)
{
    if(entity)
        m_BubbleEntityList.push_back(entity);
}

void ConvListItem::updateEntityBgType(BubbleBgViewItem::BgType bgType)
{
    for(BubbleEntity *entity : m_BubbleEntityList)
    {
        auto *bgEntity = dynamic_cast<BubbleBgEntity*>(entity);
        if(bgEntity)
            bgEntity->setBgType(bgType);
    }
}

void ConvListItem::prepareBubble(const MsgConversationItem &item, const std::string &searchWord)
{
    BubbleBgViewItem::BgType bgType = getBubbleBgType(item);
    Message::Direction direction = item.getDirection();
    if(!MsgUtils::isMms(m_Type))
    {
        addEntity(createTextEntity(bgType, direction, item.getText(), true, searchWord));
    }
    else if(m_Type == Message::MT_MMS_Noti)
    {
        std::string text = MessageDetailContent::getMmsNotiConvListItemContent(m_App, m_MsgId);
        addEntity(createTextEntity(bgType, direction, text, false, searchWord));
        addEntity(new BubbleDownloadButtonEntity(direction));
    }
    else
    {
        const MsgConvMediaList &list = item.getMediaList();
        for(int i = 0; i < list.getLength(); i++)
        {
            const MsgConvMedia &media = list.at(i);
            std::string mime = media.getMime();
            MsgMedia::Type msgMediaType = getMsgMediaTypeByMime(mime);
            std::transform(mime.begin(), mime.end(), mime.begin(), ::tolower);
            BubbleEntity *entity = nullptr;

            switch(msgMediaType)
            {
                case MsgMedia::TextType:
                    entity = createTextEntity(bgType, direction, media, searchWord);
                    break;
                case MsgMedia::ImageType:
                    // TODO: msg service corrupts thumbnail's metadata, so it lost rotation. Use getPath instead getThumbPath until fix
                    entity = new BubbleImageEntity(media, direction);
                    break;
                case MsgMedia::AudioType:
                    entity = new BubbleAudioEntity(media, bgType, direction);
                    break;
                case MsgMedia::VideoType:
                    entity = new BubbleVideoEntity(*m_WorkingDir, media, direction);
                    break;
                default:
                    if(mime == "text/x-vcalendar" || mime == "text/calendar")
                        entity =  new BubbleCalEventEntity(media, bgType, direction);
                    else if(mime == "text/x-vcard" || mime == "text/vcard")
                        entity = new BubbleContactEntity(m_App, media, bgType, direction);
                    else if(mime != "application/smil")
                        entity = new BubbleUnknownFileEntity(media, bgType, direction);
                    break;
            }
            addEntity(entity);
        }
    }
}

Evas_Object *ConvListItem::getBubbleContent()
{
    auto *bubble = new BubbleItemContainer(*getOwner());
    for(BubbleEntity *entity : m_BubbleEntityList)
    {
        BubbleViewItem *item = entity->createView(*bubble);
        bubble->append(*item, entity->getDirection());
        item->setListener(this);
    }
    bubble->go();
    bubble->show();
    return *bubble;
}

Evas_Object *ConvListItem::getThumbnail()
{
    static const int thumbSize = 80;
    return m_App.getThumbnailMaker().getThumb(*getOwner(), m_ThumbId, thumbSize);
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

std::string ConvListItem::getMsgType()
{
    // TODO: localization for "SMS"
    return MsgUtils::isMms(m_Type) ? (std::string)msg("IDS_MSGF_BODY_MMS") : msg("SMS");
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
    m_IsDraft ? showDraftListPopup() : showMainListPopup();
}

void ConvListItem::showMainListPopup()
{
    auto &listPopup = m_App.getPopupManager().getPopupList();
    listPopup.setTitle(msg("IDS_MSGF_BODY_MESSAGE_OPTIONS"));

    std::string msgText = getAllMsgText();

    if(m_NetworkStatus == Message::NS_Send_Fail)
        listPopup.appendItem(msg("IDS_MSG_OPT_RESEND"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onResendItemPressed), this);

    listPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onDeleteItemPressed), this);

    if(m_Type == Message::MT_MMS)
        listPopup.appendItem(msg("IDS_MSG_OPT_VIEW_AS_SLIDESHOW_ABB"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onSlideShowItemPressed), this);
    if(m_Type == Message::MT_MMS_Noti)
        listPopup.appendItem(msg("IDS_MSG_BUTTON_DOWNLOAD_ABB3"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onDownloadItemPressed), this);

    if(!msgText.empty())
        listPopup.appendItem(msg("IDS_MSG_OPT_COPY_TEXT"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onCopyTextItemPressed), this);

    listPopup.appendItem(msg("IDS_MSGF_OPT_FORWARD"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onForwardItemPressed), this);

    if(m_NetworkStatus == Message::NS_Send_Fail)
        listPopup.appendItem(msg("IDS_MSG_OPT_EDIT"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onEditItemPressed), this);

    if(m_Type == Message::MT_MMS)
    {
        MessageMmsRef mms = std::dynamic_pointer_cast<MessageMms>(m_App.getMsgEngine().getStorage().getMessage(m_MsgId));
        if(mms && (!mms->getAttachmentList().isEmpty() || mms->getMediaCount() > 0))
            listPopup.appendItem(msg("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onSaveAttachmentsItemPressed), this);
    }

    if(m_NetworkStatus != Message::NS_Sending && !msgText.empty() && m_Type == Message::MT_SMS)
        listPopup.appendItem(msg("IDS_MSG_OPT_COPY_TO_SIM_CARD_ABB"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onCopyToSimCardItemPressed), this);

    listPopup.appendItem(msg("IDS_MSG_OPT_VIEW_DETAILS_ABB"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onViewDetailsItemPressed), this);
    listPopup.show();
}

void ConvListItem::showDraftListPopup()
{
    auto &listPopup = m_App.getPopupManager().getPopupList();
    listPopup.setTitle(msg("IDS_MSGF_BODY_MESSAGE_OPTIONS"));
    listPopup.appendItem(msg("IDS_MSGF_OPT_EDIT_MESSAGE"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onEditItemPressed), this);
    listPopup.appendItem(msg("IDS_MSG_OPT_DELETE"), POPUPLIST_ITEM_PRESSED_CB(ConvListItem, onDeleteItemPressed), this);
    listPopup.show();
}

void ConvListItem::showFailedToSendPopup()
{
    Popup &popup = m_App.getPopupManager().getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(ConvListItem, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(ConvListItem, onCancelButtonClicked), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_RESEND_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(ConvListItem, onFailedResendButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_COULDNT_SEND_MESSAGE_ABB"));
    popup.setContent(msgt("IDS_MSG_POP_THIS_MESSAGE_WILL_BE_RESENT"));
    popup.show();
}

void ConvListItem::onDeleteItemPressed(PopupListItem &item)
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

void ConvListItem::onDownloadItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_App.getMsgEngine().getTransport().retrieveMessage(m_MsgId);
}

void ConvListItem::onAction(BubbleViewItem &item)
{
    MSG_LOG("");

    switch(item.getEntity().getType())
    {
        case BubbleEntity::DownloadButtonItem:
        {
            m_App.getMsgEngine().getTransport().retrieveMessage(m_MsgId);
            break;
        }
        case BubbleEntity::TextItem:
        {
            break;
        }
        default:
        {
            const std::string &filePath = item.getEntity().getFilePath();
            if(!filePath.empty())
                m_FileViewer.launchWithCopy(filePath);
            break;
        }
    }
}

void ConvListItem::onCopyTextItemPressed(PopupListItem &item)
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

void ConvListItem::onForwardItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    if(m_pListener)
        m_pListener->onForwardMsg(*this);
}

void ConvListItem::onResendItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    showFailedToSendPopup();
}

void ConvListItem::onSlideShowItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    if(m_pListener)
        m_pListener->onSlideShow(*this);
}

void ConvListItem::onEditItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    if(m_pListener)
        m_pListener->onEditDraftMsg(*this);
}

void ConvListItem::onSaveAttachmentsItemPressed(PopupListItem &item)
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

void ConvListItem::onCopyToSimCardItemPressed(PopupListItem &item)
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

void ConvListItem::onViewDetailsItemPressed(PopupListItem &item)
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
