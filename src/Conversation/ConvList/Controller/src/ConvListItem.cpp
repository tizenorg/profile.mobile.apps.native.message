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
#include "CallbackAssist.h"
#include "ThumbnailMaker.h"

using namespace Msg;

ConvListItem::ConvListItem(MsgConversationItem &item, App &app)
    : ConvListViewItem(getConvItemType(item))
    , m_App(app)
    , m_MsgId(item.getMsgId())
    , m_MessageText(item.getText())
    , m_IsDraft(item.isDraft())
    , m_Status(item.getStatus())
    , m_Type(item.getType())
    , m_Width(0)
    , m_Height(0)
{
}

ConvListItem::~ConvListItem()
{
}

ConvListViewItem::ConvItemType ConvListItem::getConvItemType(MsgConversationItem &item)
{
    return item.getDirection() == Message::Direction::MD_Sent ? (!item.isDraft() ? ConvItemType::Sent : ConvItemType::Draft) : ConvItemType::Received;
}

void ConvListItem::onBubbleResized(Evas_Object *obj, void *data)
{
    MSG_LOG("");
    Evas_Coord w,h;
    evas_object_geometry_get(obj, nullptr, nullptr, &w, &h);
    if(m_Height < h || m_Width < w)
    {
        m_Width = w;
        m_Height = h;
        MSG_LOG("sizes: ", m_Height, " ",m_Width);
        elm_genlist_item_update(this->getElmObjItem());
    }
}

Evas_Object *ConvListItem::getBubble()
{
    //TODO: implement getting of multimedia instead dummy content. Split to separate class (or update BubbleView)
    //Only for demo
    Evas_Object *box = elm_box_add(*getOwner());
    evas_object_event_callback_add(box, EVAS_CALLBACK_RESIZE, EVAS_EVENT_CALLBACK(ConvListItem, onBubbleResized), this);
    View::expand(box);

    Evas_Object *label = elm_label_add(box);
    elm_label_line_wrap_set(label, ELM_WRAP_MIXED);
    elm_object_part_text_set(label, nullptr, m_MessageText.c_str());
    View::expand(label);
    evas_object_show(label);
    elm_box_pack_end(box, label);

    //Dummy image
    Evas_Object *image = elm_image_add(box);
    elm_image_file_set(image, PathUtils::getResourcePath(TEST_IMG_PATH).c_str(), nullptr);
    evas_object_size_hint_min_set(image, 0, 150);
    View::expand(image);
    evas_object_show(image);
    elm_box_pack_end(box, image);

    MSG_LOG("sizes: ", m_Height, " ",m_Width);
    if(m_Height > 0 && m_Width > 0)
    {
        evas_object_size_hint_min_set(box, m_Width, m_Height);
        evas_object_size_hint_max_set(box, m_Width, m_Height);
    }
    return box;
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

void ConvListItem::onEditButtonClicked(Evas_Object *obj, void *event_info)
{
    MSG_LOG("");
}

