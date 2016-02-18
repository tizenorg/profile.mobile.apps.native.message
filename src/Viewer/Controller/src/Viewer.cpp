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

#include "Viewer.h"
#include "Logger.h"
#include "PathUtils.h"
#include "Logger.h"
#include "MsgEngine.h"
#include "App.h"
#include "PopupList.h"
#include "CallbackAssist.h"
#include "VoiceCall.h"
#include "ContactViewer.h"
#include "FileUtils.h"

#include <Elementary.h>
#include <sstream>

using namespace Msg;

Viewer::Viewer(NaviFrameController &parent, MsgId id)
    : FrameController(parent)
    , m_pLayout(nullptr)
    , m_pPlayerControl(nullptr)
    , m_pRecipPanel(nullptr)
{
    create(id);
}

Viewer::~Viewer()
{
    getApp().getContactManager().removeListener(*this);
}

void Viewer::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    updateNavibar();
    setContent(*m_pLayout);
}

void Viewer::updateNavibar()
{
    getNaviBar().clear();
    getNaviBar().setColor(NaviBar::NaviBlueColorId);
    getNaviBar().showButton(NaviPrevButtonId, true);
    if(m_Msg->getAddressList().getLength() > 1)
        getNaviBar().showButton(NaviExpandButtonId, true);
    FrameController::setNaviBarTitle(m_Msg->getAddressList());
}

void Viewer::updateRecipPanel()
{
    if(m_pRecipPanel)
        m_pRecipPanel->update(m_Msg->getAddressList());
}

void Viewer::create(MsgId id)
{
    m_Msg = std::dynamic_pointer_cast<MessageMms>(getMsgEngine().getStorage().getMessage(id));
    if(!m_Msg)
    {
        MSG_LOG_ERROR("Can't get message by id");
        // Create empty message:
        m_Msg = getMsgEngine().getComposer().createMms();
    }

    getApp().getContactManager().addListener(*this);
    createLayout();
    createPlayerControl();
    createRecipPanel();
    setHwButtonListener(*m_pLayout, this);
}

void Viewer::createLayout()
{
    if(!m_pLayout)
    {
        m_pLayout = new ViewerLayout(getParent());
        m_pLayout->setListener(this);
        m_pLayout->show();
    }
}

void Viewer::createPlayerControl()
{
    if(!m_pPlayerControl)
    {
        m_pPlayerControl = new PlayerControl(*m_pLayout);
        m_pPlayerControl->setStartTime("00:00");
        m_pPlayerControl->setEndTime("00:00"); // For test
        m_pPlayerControl->setListener(this);
        m_pPlayerControl->show();
        m_pPlayerControl->setProgress(0.0);
        m_pLayout->setPlayerControl(*m_pPlayerControl);
    }
}

void Viewer::createRecipPanel()
{
    if(!m_pRecipPanel && m_Msg->getAddressList().getLength() > 1)
    {
        m_pRecipPanel = new MbeRecipients(*m_pLayout, getApp());
        m_pRecipPanel->addSmartCb("item,clicked", SMART_CALLBACK(Viewer, onRecipItemClicked), this);
        m_pRecipPanel->show();
        m_pLayout->setRecipients(*m_pRecipPanel);
        updateRecipPanel();
    }
}

std::string Viewer::createMessageText() const
{
    MsgPageList &pageList = m_Msg->getPageList();
    std::string result;

    int size = pageList.getLength();
    for(int i = 0; i < size; ++i)
    {
        MsgMediaList &mediaList = pageList.at(i).getMediaList();

        int sizeList = mediaList.getLength();
        for(int j = 0; j < sizeList; ++j)
        {
            if(mediaList.at(j).getType() == MsgMedia::SmilText)
            {
                result += FileUtils::readTextFile(mediaList.at(j).getFilePath());
                if(i < size - 1)
                    result.append("\n");
            }
        }
    }
    return result;
}

void Viewer::naviExpandButtonHandler()
{
    MSG_LOG("");
    if(m_pRecipPanel)
    {
        bool isRecipInvisible = !m_pLayout->isRecipientsVisible();
        m_pLayout->showRecipients(isRecipInvisible);
        getNaviBar().setDownButtonState(isRecipInvisible);
    }
}

void Viewer::naviCenterButtonHandler()
{
    MSG_LOG("");
    if(m_pRecipPanel)
    {
        naviExpandButtonHandler();
    }
    else
    {
        const MsgAddressList &addressList = m_Msg->getAddressList();
        if(!addressList.isEmpty())
            recipientClickHandler(addressList[0].getAddress());
    }
}

void Viewer::naviPrevButtonHandler()
{
    getParent().pop();
}

void Viewer::showRecipPopup(const std::string &title)
{
    PopupList &popup = getApp().getPopupManager().getPopupList();
    popup.setTitle(title);
    popup.setAutoDismissBlockClickedFlag(true);
    popup.appendItem(msg("IDS_MSG_OPT_MAKE_VOICE_CALL"),  POPUPLIST_ITEM_PRESSED_CB(Viewer, onMakeVoiceItemPressed), this);
    popup.appendItem(msg("IDS_MSG_OPT_CREATE_CONTACT_ABB"),  POPUPLIST_ITEM_PRESSED_CB(Viewer, onCreateContactItemPressed), this);
    popup.appendItem(msg("IDS_MSG_OPT_UPDATE_CONTACT"),  POPUPLIST_ITEM_PRESSED_CB(Viewer, onUpdateContactItemPressed), this);
    popup.show();
}

void Viewer::recipientClickHandler(const std::string &address)
{
    MSG_LOG("");
    m_SelectedAddress = address;
    ContactPersonAddressRef contactPersonAddress = getApp().getContactManager().getContactPersonAddress(address);
    if(contactPersonAddress)
        ContactViewer::launch(contactPersonAddress->getPersonId());
    else
        showRecipPopup(address);
}

void Viewer::onHwBackButtonClicked()
{
    getParent().pop();
}

void Viewer::onHwMoreButtonClicked()
{
    PopupList &popup = getApp().getPopupManager().getPopupList();
    popup.setAutoDismissBlockClickedFlag(true);
    popup.appendItem(msg("IDS_MSG_OPT_DELETE"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onDeleteItemPressed), this);

    if(!createMessageText().empty())
        popup.appendItem(msg("IDS_MSG_OPT_COPY_TEXT"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onCopyTextItemPressed), this);

    popup.appendItem(msg("IDS_MSGF_OPT_FORWARD"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onForwardItemPressed), this);

    if(!m_Msg->getAttachmentList().isEmpty())
        popup.appendItem(msg("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onSaveAttachmentsItemPressed), this);
    popup.show();
}

void Viewer::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    MSG_LOG("");
    switch(buttonId)
    {
        case NaviCenterButtonId:
            naviCenterButtonHandler();
            break;

        case NaviPrevButtonId:
            naviPrevButtonHandler();
            break;

        case NaviExpandButtonId:
            naviExpandButtonHandler();
            break;

        default:
            break;
    };
}

void Viewer::onPlayClicked()
{
    MSG_LOG("");
    m_pPlayerControl->setPlayState(PlayerControl::PauseState);
}

void Viewer::onPauseClicked()
{
    MSG_LOG("");
    m_pPlayerControl->setPlayState(PlayerControl::PlayState);
}

void Viewer::onNextClicked()
{
    MSG_LOG("");
}

void Viewer::onPrevClicked()
{
    MSG_LOG("");
}

void Viewer::onContactChanged()
{
    MSG_LOG("");
    updateNavibar();
    updateRecipPanel();
}

void Viewer::onLayoutTocuh()
{
    MSG_LOG("");
    m_pLayout->showPlayerControl(!m_pLayout->isPlayerControlVisible());
}

void Viewer::onMakeVoiceItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    VoiceCall::launch(m_SelectedAddress);
}

void Viewer::onCreateContactItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_ContactEditor.launch(m_SelectedAddress, ContactEditor::CreateOp);
}

void Viewer::onUpdateContactItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_ContactEditor.launch(m_SelectedAddress, ContactEditor::EditOp);
}

void Viewer::onDeleteItemPressed(PopupListItem &item)
{
    item.getParent().destroy();
    Popup &popup = getApp().getPopupManager().getPopup();
    popup.setAutoDismissBlockClickedFlag(true);
    popup.addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(Viewer, onCancelButtonClicked), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_REMOVE_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(Viewer, onDeleteButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_DELETE"));
    popup.setContent(msgt("IDS_MSG_POP_1_MESSAGE_WILL_BE_DELETED"));
    popup.show();
}

void Viewer::onCancelButtonClicked(Popup &popup, int buttonId)
{
    popup.destroy();
}

void Viewer::onDeleteButtonClicked(Popup &popup, int buttonId)
{
    popup.destroy();
    getApp().getMsgEngine().getStorage().deleteMessage(m_Msg->getId());
    getParent().pop();
}

void Viewer::onCopyTextItemPressed(PopupListItem &item)
{
    item.getParent().destroy();
    std::string msgText = createMessageText();
    elm_cnp_selection_set(this->getContent(), ELM_SEL_TYPE_CLIPBOARD, ELM_SEL_FORMAT_TEXT, msgText.c_str(), msgText.length());
}

void Viewer::onForwardItemPressed(PopupListItem &item)
{
    item.getParent().destroy();

    Conversation *conv = new Conversation(getParent());
    conv->setListener(this);
    conv->forwardMsg(m_Msg->getId());
    getParent().push(*conv);
}

void Viewer::onSaveAttachmentsItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy(); // TODO: after save attachments menu will be implemented
}

void Viewer::onRecipItemClicked(Evas_Object *obj, void *eventInfo)
{
    MbeRecipientItem *item = ViewItem::staticCast<MbeRecipientItem*>(eventInfo);
    recipientClickHandler(item->getAddress());
}

void Viewer::onConversationSentMessage()
{
    getParent().pop();
}
