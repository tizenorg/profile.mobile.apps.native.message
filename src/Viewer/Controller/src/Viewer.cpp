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
#include "SaveAttachmentsPopup.h"
#include "FileShare.h"

#include <sstream>
#include <iomanip>

using namespace Msg;

namespace
{
    std::string makeTimeStr(int timeSec)
    {
        const int digitCount = 2;
        int min = timeSec / 60;
        int sec = timeSec % 60;

        std::ostringstream ss;
        ss.width();
        ss << std::setw(digitCount) << std::setfill('0') << min << ":" << std::setw(digitCount) << std::setfill('0') << sec;
        return ss.str();
    }
}

Viewer::Viewer(NaviFrameController &parent, MsgId id)
    : FrameController(parent)
    , m_pLayout(nullptr)
    , m_pPlayerControl(nullptr)
    , m_pRecipPanel(nullptr)
    , m_pSmilPlayer(nullptr)
    , m_pSubjectLayout(nullptr)
{
    create(id);
}

Viewer::~Viewer()
{
    MSG_LOG("");
    getApp().getContactManager().removeListener(*this);
    getApp().getSysSettingsManager().removeListener(*this);
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

    createLayout();
    createSubjectLayout();
    createPlayerControl();
    createRecipPanel();
    createSmilPlayer();

    updateButtonState();
    updatePlayPos();

    getApp().getSysSettingsManager().addListener(*this);
    getApp().getContactManager().addListener(*this);
    setHwButtonListener(*m_pLayout, this);

    m_pSmilPlayer->start();
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

void Viewer::createSubjectLayout()
{
    if(!m_pSubjectLayout)
    {
        m_pSubjectLayout = new SubjectLayout(*m_pLayout);
        m_pSubjectLayout->setNumberOfPages(m_Msg->getPageList().getLength());
        m_pSubjectLayout->show();
        m_pLayout->setSubject(*m_pSubjectLayout);
        updateSubject();
    }
}

void Viewer::createPlayerControl()
{
    if(!m_pPlayerControl)
    {
        m_pPlayerControl = new PlayerControl(*m_pLayout);

        m_pPlayerControl->setListener(this);
        m_pPlayerControl->show();
        m_pPlayerControl->setProgress(0.0);
        m_pLayout->setPlayerControl(*m_pPlayerControl);

        createSmilPlayer();
        m_pPlayerControl->setStartTime(makeTimeStr(0));
        m_pPlayerControl->setEndTime(makeTimeStr(m_pSmilPlayer->getDuration()));
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

void Viewer::createSmilPlayer()
{
    if(!m_pSmilPlayer)
    {
        m_pSmilPlayer = new SmilPlayer(*m_pLayout, *m_Msg);
        m_pSmilPlayer->setListener(this);
        m_pSmilPlayer->show();
        m_pLayout->setBody(*m_pSmilPlayer);
    }
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

void Viewer::updatePlayPos()
{
    double pos = m_pSmilPlayer->getPosition();
    int sec = m_pSmilPlayer->getDuration() * pos + 0.5;
    m_pPlayerControl->setProgress(pos);
    m_pPlayerControl->setStartTime(makeTimeStr(sec));
}

void Viewer::updateSubject()
{
    if(m_pSubjectLayout)
    {
        std::string subject = m_Msg->getSubject();
        if(subject.empty())
            subject = msg("IDS_MSGF_BODY_NO_SUBJECT");
        m_pSubjectLayout->setSubjectText(subject);
    }
}

void Viewer::updateButtonState()
{
    auto playPauseState = PlayerControl::PauseState;
    bool isPlaying = m_pSmilPlayer->getState() == SmilPlayer::PlayState;

    if(m_pSmilPlayer->getState() == SmilPlayer::StopState)
        playPauseState = PlayerControl::PlayState;

    m_pPlayerControl->setPlayState(playPauseState);
    m_pPlayerControl->enableNextButton(m_pSmilPlayer->probeNextPage() && isPlaying);
    m_pPlayerControl->enablePrevButton(m_pSmilPlayer->probePrevPage() && isPlaying);
}

void Viewer::onHwBackButtonClicked()
{
    getParent().pop();
}

void Viewer::onHwMoreButtonClicked()
{
    m_pSmilPlayer->stop();

    PopupList &popup = getApp().getPopupManager().getPopupList();
    popup.setAutoDismissBlockClickedFlag(true);
    popup.appendItem(msg("IDS_MSG_OPT_DELETE"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onDeleteItemPressed), this);

    if(!m_Msg->getText().empty())
        popup.appendItem(msg("IDS_MSG_OPT_COPY_TEXT"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onCopyTextItemPressed), this);

    popup.appendItem(msg("IDS_MSGF_OPT_FORWARD"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onForwardItemPressed), this);

    bool hasAttachment = !m_Msg->getAttachmentList().isEmpty() || m_Msg->getMediaCount() > 0;
    if(hasAttachment)
    {
        popup.appendItem(msg("IDS_MSG_OPT_SAVE_ATTACHMENTS_ABB"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onSaveAttachmentsItemPressed), this);
        // TODO: localization
        popup.appendItem(msg("Share"), POPUPLIST_ITEM_PRESSED_CB(Viewer, onShareItemPressed), this);
    }

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
    m_pSmilPlayer->start();
}

void Viewer::onPauseClicked()
{
    MSG_LOG("");
    m_pSmilPlayer->stop();
}

void Viewer::onNextClicked()
{
    MSG_LOG("");
    m_pSmilPlayer->nextPage();
}

void Viewer::onPrevClicked()
{
    MSG_LOG("");
    m_pSmilPlayer->prevPage();
}

void Viewer::onContactChanged()
{
    MSG_LOG("");
    updateNavibar();
}

void Viewer::onLayoutTap()
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
    std::string msgText = m_Msg->getText();
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
    item.getParent().destroy();
    SaveAttachmentsPopup *popup = new SaveAttachmentsPopup(getApp(), *m_Msg.get());
    getApp().getPopupManager().reset(*popup);
    popup->show();
}

void Viewer::onShareItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();

    SmilPage *page = m_pSmilPlayer->getCurrentPage();
    if(page)
        FileShare::launch(page->getAttachments());
}

void Viewer::onRecipItemClicked(Evas_Object *obj, void *eventInfo)
{
    MSG_LOG("");
    MbeRecipientItem *item = ViewItem::staticCast<MbeRecipientItem*>(eventInfo);
    recipientClickHandler(item->getAddress());
}

void Viewer::onConversationSentMessage()
{
    getParent().pop();
}

void Viewer::onSmilPlayerStateChanged()
{
    MSG_LOG("Smil player state: ", m_pSmilPlayer->getState());
    updateButtonState();
}

void Viewer::onSmilPlayerPageChanged()
{
    MSG_LOG("");
    updateButtonState();
    updatePlayPos();
    m_pSubjectLayout->setPageIndex(m_pSmilPlayer->getCurrentPageIndex() + 1);
}

void Viewer::onSmilPlayerTick()
{
    MSG_LOG("");
    updatePlayPos();
}

void Viewer::onLanguageChanged()
{
    MSG_LOG("");
    updateSubject();
}
