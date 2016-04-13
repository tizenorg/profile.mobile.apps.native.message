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
#include "Logger.h"
#include "PathUtils.h"
#include "Scroller.h"
#include "App.h"
#include "Message.h"
#include "Logger.h"
#include "LangUtils.h"
#include "CallbackAssist.h"
#include "ContactViewer.h"
#include "VoiceCall.h"
#include "Viewer.h"
#include "PopupRecipientListItem.h"

#include <Elementary.h>
#include <sstream>
#include <notification.h>

using namespace Msg;

// TODO: move to common part
std::string makeKbStr(long long bytes)
{
    const long long kb = 1000; // Bytes in kb
    long long sizeKb = 0;
    if(bytes <= kb)
    {
        sizeKb = 1;
    }
    else
    {
        sizeKb =  bytes / kb;
        long long sizeB = bytes % kb;
        if(sizeB >= (kb / 2))
            ++sizeKb;
    }
    return std::to_string(sizeKb) + " " + (std::string)msg("IDS_MSGF_BODY_MSGSIZE_KB");
}

Conversation::Conversation(NaviFrameController &parent)
    : FrameController(parent)
    , m_Mode(InitMode)
    , m_pLayout(nullptr)
    , m_pMsgInputPanel(nullptr)
    , m_pBody(nullptr)
    , m_pRecipPanel(nullptr)
    , m_pContactsList(nullptr)
    , m_ThreadId()
    , m_IsMms(false)
    , m_pConvList(nullptr)
    , m_AttachPanel(getApp())
    , m_pListener(nullptr)
{
    create();
}

Conversation::~Conversation()
{
    // Call before delete all children:
    MSG_LOG("");
    getApp().getContactManager().removeListener(*this);
    if(m_pBody)
        m_pBody->setListener(nullptr);
    if(m_pRecipPanel)
        m_pRecipPanel->setListener(nullptr);
    if(m_pContactsList)
        m_pContactsList->setListener(nullptr);
    m_AttachPanel.setListener(nullptr);
}

void Conversation::execCmd(const AppControlComposeRef &cmd)
{
    if(!isRecipExists() && !isBodyEmpty() && m_Mode == NewMessageMode) // Check if we can save draft to avoid losing data
    {
        m_DefferedCmd.composeCmd = cmd;
        showNoRecipPopup();
        return;
    }
    else
    {
        saveDraftMsg();
    }

    setThreadId(ThreadId());
    if(m_pRecipPanel)
        m_pRecipPanel->execCmd(cmd);
    if(m_pBody)
        m_pBody->execCmd(cmd);

    if(!isRecipExists())
        m_pRecipPanel->setEntryFocus(true);
    else
        m_pBody->setFocus(true);
}

void Conversation::execCmd(const AppControlDefaultRef &cmd)
{
    if(!isRecipExists() && !isBodyEmpty() && m_Mode == NewMessageMode) // Check if we can save draft to avoid losing data
    {
        m_DefferedCmd.defaultCmd = cmd;
        showNoRecipPopup();
        return;
    }
    else
    {
        saveDraftMsg();
    }

    if(cmd->getDefaultType() == AppControlDefault::NotificationType && getMsgEngine().getStorage().getUnreadThreadCount() > 1)
    {
        getParent().pop();
        return;
    }

    setThreadId(getMsgEngine().getStorage().getMessage(cmd->getMessageId())->getThreadId());
    if(cmd->getDefaultType() == AppControlDefault::ReplyType)
        m_pBody->setFocus(true);
}

void Conversation::create()
{
    m_WorkingDir = std::make_shared<WorkingDir>();
    createMainLayout(getParent());
    createMsgInputPanel(*m_pLayout);
    createBody(*m_pMsgInputPanel);
    updateMsgInputPanel();

    getApp().getContactManager().addListener(*this);
    setHwButtonListener(*m_pLayout, this);
    m_AttachPanel.setListener(this);
}

void Conversation::markAsRead()
{
    if(m_ThreadId.isValid())
        getMsgEngine().getStorage().setReadStatus(m_ThreadId);
}

void Conversation::recipientClickHandler(const std::string &address)
{
    MSG_LOG("");
    MSG_ASSERT(m_Mode != InitMode, "m_Mode is in initial state");
    ContactPersonAddressRef contactPersonAddress = getApp().getContactManager().getContactPersonAddress(address);
    if(contactPersonAddress)
    {
        int selectedPersonId = contactPersonAddress->getPersonId();
        if(m_Mode == NewMessageMode)
        {
            MbeRecipientItem *pItem = m_pRecipPanel->getSelectedItem();
            if(pItem)
            {
                showSavedRecipientPopup(pItem->getDispName(), selectedPersonId);
            }
        }
        else if(m_Mode == ConversationMode)
        {
            ContactViewer::launch(selectedPersonId);
        }
    }
    else
    {
        showUnsavedRecipientPopup(address);
    }
}

void Conversation::showSavedRecipientPopup(const std::string &title, int personId)
{
    PopupList &popup = createPopupList(title);
    popup.appendItem(msg("IDS_MSGF_OPT_REMOVE"), POPUPLIST_ITEM_PRESSED_CB(Conversation, onRemoveItemPressed), this);
    popup.appendItem(msg("IDS_MSG_OPT_EDIT"), POPUPLIST_ITEM_PRESSED_CB(Conversation, onEditItemPressed), this);
    popup.appendItem(*new PopupPersonIdListItem(popup, msg("IDS_MSG_OPT_VIEW_CONTACT_DETAILS_ABB"), personId,
            POPUPLIST_ITEM_PRESSED_CB(Conversation, onViewContactDetailsItemPressed), this));
    popup.show();
}

void Conversation::showUnsavedRecipientPopup(const std::string &address)
{
    PopupList &popup = createPopupList(address);
    if(m_Mode == NewMessageMode)
    {
        popup.appendItem(*new PopupAddressListItem(popup, msg("IDS_MSGF_OPT_REMOVE"), address,
                POPUPLIST_ITEM_PRESSED_CB(Conversation, onRemoveItemPressed), this));

        popup.appendItem(*new PopupAddressListItem(popup, msg("IDS_MSG_OPT_EDIT"), address,
                POPUPLIST_ITEM_PRESSED_CB(Conversation, onEditItemPressed), this));
    }
    else if(m_Mode == ConversationMode)
    {
        popup.appendItem(*new PopupAddressListItem(popup, msg("IDS_MSG_OPT_MAKE_VOICE_CALL"), address,
                POPUPLIST_ITEM_PRESSED_CB(Conversation, onMakeVoiceItemPressed), this));
    }

    popup.appendItem(*new PopupAddressListItem(popup, msg("IDS_MSG_OPT_CREATE_CONTACT_ABB"), address,
                POPUPLIST_ITEM_PRESSED_CB(Conversation, onCreateContactItemPressed), this));

    popup.appendItem(*new PopupAddressListItem(popup, msg("IDS_MSG_OPT_UPDATE_CONTACT"), address,
                POPUPLIST_ITEM_PRESSED_CB(Conversation, onUpdateContactItemPressed), this));
    popup.show();
}

void Conversation::contactChangedHandler()
{
    updateNavibar();
}

void Conversation::navigateTo(MsgId msgId)
{
    m_pConvList->navigateTo(msgId);
}

void Conversation::navigateToLastMsg()
{
    m_pConvList->navigateToLastMsg();
}

void Conversation::setThreadId(ThreadId id, const std::string &searchWord)
{
    m_ThreadId = id;
    setMode(m_ThreadId.isValid() ? ConversationMode : NewMessageMode);
    m_pBody->clear();
    if(m_pRecipPanel)
    {
        m_pRecipPanel->clear();
        m_pRecipPanel->update(m_ThreadId);
    }
    if(m_pConvList)
        m_pConvList->setThreadId(m_ThreadId, searchWord);
    markAsRead();
}

void Conversation::setListener(IConversationListener *listener)
{
    m_pListener = listener;
}

void Conversation::forwardMsg(MsgId id)
{
    MessageRef msg = getMsgEngine().getStorage().getMessage(id);
    if(msg)
    {
        setThreadId(ThreadId());
        if(m_pBody)
            m_pBody->write(*msg);
        m_pRecipPanel->setEntryFocus(true);
    }
}

MsgAddressListRef Conversation::getAddressList()
{
    return getMsgEngine().getStorage().getAddressList(m_ThreadId);
}

void Conversation::setMode(Mode mode)
{
    if(m_Mode == mode)
        return;

    switch(mode)
    {
        case NewMessageMode:
            setNewMessageMode();
            break;

        case ConversationMode:
            setConversationMode();
            break;

        case InitMode:
            break;

        default:
            MSG_LOG_ERROR("Wrong mode");
            break;
    }
}

void Conversation::setNewMessageMode()
{
    m_Mode = NewMessageMode;

    createRecipPanel(*m_pLayout);
    createContactList(*m_pLayout);
    destroyConvList();
    updateNavibar();

    m_pRecipPanel->update(m_ThreadId);
    m_pRecipPanel->showMbe(!m_pRecipPanel->isMbeEmpty());
    m_pRecipPanel->showEntry(true);
    m_pRecipPanel->setEntryFocus(true);
}

void Conversation::setConversationMode()
{
    MSG_LOG("");

    m_Mode = ConversationMode;
    createConvList(*m_pLayout);

    updateNavibar();

    MsgAddressListRef addressList = getAddressList();
    if(addressList && addressList->getLength() > 1)
    {
        createRecipPanel(*m_pLayout);
        m_pRecipPanel->showEntry(false);
        m_pRecipPanel->update(*addressList);
    }
    else
    {
        destroyRecipPanel();
    }
    destroyContactList();
}

void Conversation::createMainLayout(Evas_Object *parent)
{
    m_pLayout = new ConversationLayout(parent);
    m_pLayout->show();
    m_pLayout->expand();
}

void Conversation::createConvList(Evas_Object *parent)
{
    if(!m_pConvList)
    {
        m_pConvList = new ConvList(*m_pLayout, getApp(), m_WorkingDir);
        m_pConvList->setListener(this);
        m_pConvList->show();
        m_pLayout->setConvList(*m_pConvList);
    }
}

void Conversation::destroyConvList()
{
    if(m_pConvList)
    {
        m_pConvList->destroy();
        m_pConvList = nullptr;
    }
}

void Conversation::createRecipPanel(Evas_Object *parent)
{
    if(!m_pRecipPanel)
    {
        m_pRecipPanel = new ConvRecipientsPanel(parent, getApp());
        m_pRecipPanel->setListener(this);
        m_pRecipPanel->show();
        m_pLayout->setRecipientPanel(*m_pRecipPanel);
        m_pLayout->setRecipientRect(m_pRecipPanel->getAreaRect());
    }
}

void Conversation::destroyRecipPanel()
{
    if(m_pRecipPanel)
    {
        m_pRecipPanel->destroy();
        m_pRecipPanel = nullptr;
    }
}

void Conversation::createContactList(Evas_Object *parent)
{
    if(!m_pContactsList)
    {
        m_pContactsList = new ConvContactList(parent, getApp());
        m_pContactsList->setListener(this);
        m_pContactsList->show();
        m_pLayout->setContactList(*m_pContactsList);
    }
}

void Conversation::destroyContactList()
{
    m_pLayout->showContactList(false);
    if(m_pContactsList)
    {
        m_pContactsList->destroy();
        m_pContactsList = nullptr;
    }
}

void Conversation::createMsgInputPanel(Evas_Object *parent)
{
    if(!m_pMsgInputPanel)
    {
        m_pMsgInputPanel = new MessageInputPanel(parent);
        m_pMsgInputPanel->setListener(this);
        m_pMsgInputPanel->show();
        m_pLayout->setMsgInputPanel(*m_pMsgInputPanel);
    }
}

void Conversation::createBody(Evas_Object *parent)
{
    if(!m_pBody)
    {
        m_pBody = new Body(getApp(), m_WorkingDir);
        m_pBody->create(*m_pMsgInputPanel);
        m_pBody->setListener(this);
        m_pBody->show();
        assert(m_pMsgInputPanel);
        if(m_pMsgInputPanel)
            m_pMsgInputPanel->setEntry(*m_pBody);
    }
}

void Conversation::write(const Message &msg)
{
    m_pBody->write(msg);
    if(m_pRecipPanel)
        m_pRecipPanel->write(msg);
}

void Conversation::read(Message &msg)
{
    m_pBody->read(msg);
    readMsgAddress(msg);
}

void Conversation::readMsgAddress(Message &msg)
{
    if(m_ThreadId.isValid())
    {
        MsgAddressListRef addressList = getAddressList();
        if(addressList)
            msg.addAddresses(*addressList);
    }
    else
    {
        if(m_pRecipPanel)
            m_pRecipPanel->read(msg);
    }
}

void Conversation::sendMessage()
{
    if(!m_ThreadId.isValid() && m_pRecipPanel->isMbeEmpty())
    {
        showAddRecipPopup();
        m_pRecipPanel->setEntryFocus(true);
        return;
    }

    auto msg = getMsgEngine().getComposer().createMessage(m_IsMms ? Message::MT_MMS : Message::MT_SMS);
    read(*msg);
    MSG_LOG("m_ThreadId = ", m_ThreadId);
    MsgTransport::SendResult sendRes = getMsgEngine().getTransport().sendMessage(*msg, &m_ThreadId);

    MSG_LOG("Send result = ", sendRes);
    MSG_LOG("m_ThreadId = ", m_ThreadId);

    if(sendRes == MsgTransport::SendSuccess && m_ThreadId.isValid())
    {
        if(m_Mode == NewMessageMode)
            setMode(ConversationMode);

        m_pConvList->setThreadId(m_ThreadId);
        m_IsMms = false;
        m_pBody->clear();
        m_pBody->setFocus(true);
    }
    else
    {
        showSendResultPopup(sendRes);
    }

    if(m_pListener)
        m_pListener->onConversationSentMessage();
}

void Conversation::saveDraftMsg()
{
    MSG_LOG("");
    if(!isBodyEmpty())
    {
        MessageRef msg = getMsgEngine().getComposer().createMessage(m_IsMms ? Message::MT_MMS : Message::MT_SMS);

        if(msg)
        {
            read(*msg);
            msg->setMessageStorageType(Message::MS_Phone);
            MsgId msgId = getMsgEngine().getStorage().saveMessage(*msg, false);
            MSG_LOG("Draft message id = ", msgId);
        }
    }
}

void Conversation::editDraftMsg(MsgId id)
{
    MessageRef msg = getMsgEngine().getStorage().getMessage(id);
    if(msg)
    {
        saveDraftMsg(); // TODO: Check case if edit single(int ConvList) when Body not empty
        if(m_pConvList->getMessageCount() <= 1)
            setThreadId(ThreadId());

        write(*msg);
        getMsgEngine().getStorage().deleteMessage(id);
        m_pBody->setFocus(true);
    }
}

void Conversation::notifyConvertMsgType()
{
    std::string notifText = m_IsMms ?
            msg("IDS_MSGF_POP_CONVERTING_TO_MULTIMEDIA_MESSAGE_ING") :
            (std::string)msg("IDS_MSGF_POP_CONVERTING_TO_TEXT_MESSAGE_ING");

    notification_status_message_post(notifText.c_str());
}

void Conversation::convertMsgTypeHandler()
{
    MSG_LOG("Is MMS: ", m_IsMms);
    notifyConvertMsgType();
}

void Conversation::checkAndSetMsgType()
{
    // Body:
    bool isMms = m_pBody->isMms();

    // Recipients:
    if(!isMms && m_pRecipPanel && !m_pRecipPanel->isMbeEmpty())
        isMms = m_pRecipPanel->isMms();

    if(isMms != m_IsMms)
    {
        m_IsMms = isMms;
        convertMsgTypeHandler();
    }
}

void Conversation::resetDefferedCmd()
{
    m_DefferedCmd.composeCmd.reset();
    m_DefferedCmd.defaultCmd.reset();
}

bool Conversation::isDefferedCmd() const
{
    return m_DefferedCmd.composeCmd || m_DefferedCmd.defaultCmd;
}

bool Conversation::isRecipExists() const
{
    return m_pRecipPanel && !m_pRecipPanel->isMbeEmpty();
}

bool Conversation::isBodyEmpty() const
{
    return m_pBody && m_pBody->isEmpty();
}

void Conversation::navigateToSlideShow(MsgId id)
{
    Viewer *viewer = new Viewer(getParent(), id);
    getParent().push(*viewer);
}

PopupList &Conversation::createPopupList(const std::string &title)
{
    PopupList &popup = getApp().getPopupManager().getPopupList();
    popup.setTitle(title);
    popup.setAutoDismissBlockClickedFlag(true);
    return popup;
}

void Conversation::showNoRecipPopup()
{
    Popup &popup = getApp().getPopupManager().getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Conversation, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_CANCEL_ABB"), Popup::CancelButtonId, POPUP_BUTTON_CB(Conversation, onNoRecipCancelButtonClicked), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_DISCARD_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(Conversation, onNoRecipDiscardButtonClicked), this);
    popup.setTitle(msgt("IDS_MSG_HEADER_DISCARD_MESSAGE_M_CLOSE_ABB"));
    popup.setContent(msgt("IDS_MSG_POP_YOUR_MESSAGE_WILL_BE_DISCARDED_NO_RECIPIENTS_HAVE_BEEN_SELECTED"));
    popup.show();
}

void Conversation::showAddRecipPopup()
{
    Popup &popup = getApp().getPopupManager().getPopup();
    popup.setContent(msgt("IDS_MSG_TPOP_ADD_RECIPIENTS"));
    popup.setTimeOut(2.0);
    popup.show();
}

void Conversation::showSendResultPopup(MsgTransport::SendResult result)
{
    if(result == MsgTransport::SendSuccess)
        return;

    const char *strId = nullptr;
    switch(result)
    {
        case MsgTransport::SendNoSIM:
            strId = "IDS_MSG_BODY_UNABLE_TO_SEND_THIS_MESSAGE_INSERT_YOUR_SIM_CARD_AND_TRY_AGAIN";
            break;
        default:
            strId = "IDS_MSGC_BODY_UNABLE_TO_SEND_MESSAGE";
    };

    auto &popupMngr = getApp().getPopupManager();
    Popup &popup = popupMngr.getPopup();
    popup.addEventCb(EVAS_CALLBACK_DEL, EVAS_EVENT_CALLBACK(Conversation, onPopupDel), this);
    popup.addButton(msgt("IDS_MSG_BUTTON_OK_ABB"), Popup::OkButtonId, POPUP_BUTTON_CB(Conversation, onMsgSendErrorButtonClicked), this);
    popup.setContent(msgt(strId));
    popup.show();
}

void Conversation::showMainPopup()
{
    PopupList &popup = getApp().getPopupManager().getPopupList();
    popup.appendItem(msg("IDS_MSG_OPT_DELETE"), POPUPLIST_ITEM_PRESSED_CB(Conversation, onDeleteItemPressed), this);
    popup.appendItem(msg("IDS_MSG_OPT_ADD_RECIPIENTS_ABB"), POPUPLIST_ITEM_PRESSED_CB(Conversation, onAddRecipientsItemPressed), this);
    popup.show();
}

void Conversation::onKeyDown(ConvRecipientsPanel &panel, Evas_Event_Key_Down &ev)
{
    if(ev.keyname)
    {
        if((strcmp(ev.keyname, "Return") == 0))
        {
            m_pBody->setFocus(true);
        }
        else
        {
            m_pContactsList->setSearchWorld(m_pRecipPanel->getEntryText());
            m_pContactsList->requestSearch();
        }
    }
}

void Conversation::onEntryFocusChanged(ConvRecipientsPanel &panel)
{
    if(!m_pRecipPanel->getEntryFocus())
        m_pContactsList->clear();
}

void Conversation::onMbeChanged(ConvRecipientsPanel &panel)
{
    MSG_LOG("");
    checkAndSetMsgType();
}

void Conversation::onItemClicked(ConvRecipientsPanel &panel, MbeRecipientItem &item)
{
    MSG_LOG("");
    recipientClickHandler(item.getAddress());
}

void Conversation::onChanged(Body &body)
{
    updateMsgInputPanel();
    checkAndSetMsgType();
}

void Conversation::updateMsgInputPanel()
{
    if(m_pBody->isMms())
    {
        // Mms:
        m_pMsgInputPanel->setCounter(makeKbStr(m_pBody->getMsgSize()));
    }
    else
    {
        // Sms:
        std::ostringstream ss;
        const MsgTextMetric &size = m_pBody->getTextMetric();
        ss << size.charsLeft << "/" << size.segmentsCount;
        m_pMsgInputPanel->setCounter(ss.str());
    }

    bool disabledButton = m_pBody->isEmpty();
    m_pMsgInputPanel->disabledButton(MessageInputPanel::SendButtonId, disabledButton);
}

void Conversation::updateNavibar()
{
    NaviBar &naviBar = getNaviBar();
    naviBar.clear();
    naviBar.setColor(NaviBar::NaviWhiteColorId);

    if(m_Mode == NewMessageMode)
    {
        naviBar.setTitle(msgt("IDS_MSGF_POP_NEW_MESSAGE"));
        naviBar.showButton(NaviPrevButtonId, true);
    }
    else if(m_Mode == ConversationMode)
    {
        if(m_pConvList->getMode() == ConvList::SelectMode)
        {
            naviBar.setTitle(msgt("IDS_MSG_OPT_DELETE"));
            naviBar.showButton(NaviCancelButtonId, true);
            naviBar.showButton(NaviOkButtonId, true);
            naviBar.disabledButton(NaviOkButtonId, true);
        }
        else
        {
            naviBar.showButton(NaviPrevButtonId, true);
            MsgAddressListRef addressList = getAddressList();
            if(addressList)
            {
                naviBar.showButton(NaviCenterButtonId, true);
                FrameController::setNaviBarTitle(*addressList);
            }
        }
    }
}

void Conversation::onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id)
{
    MSG_LOG("MessageInputPanel: button clicked: id = ", id);

    switch(id)
    {
        case MessageInputPanel::AddButtonId:
            m_AttachPanel.show(true);
            break;
        case MessageInputPanel::SendButtonId:
            sendMessage();
            m_AttachPanel.show(false);
            break;
        default:
            break;
    }
}

void Conversation::onContactSelected(ContactListItem &item)
{
    m_pRecipPanel->appendItem(item.getRecipient());
    m_pRecipPanel->clearEntry();
    m_pContactsList->clear();
}

void Conversation::onContactListChanged()
{
    if(m_pContactsList)
        m_pLayout->showContactList(!m_pContactsList->isEmpty());
}

void Conversation::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    updateNavibar();
    setContent(*m_pLayout);
}

void Conversation::onHwBackButtonClicked()
{
    MSG_LOG("");
    if(m_pConvList && m_pConvList->getMode() == ConvList::SelectMode)
    {
        m_pConvList->setMode(ConvList::NormalMode);
        updateNavibar();
        return;
    }
    if(m_pRecipPanel)
    {
        if(m_pRecipPanel->isMbeVisible() || m_pRecipPanel->getItemsCount() == 0)
            m_pRecipPanel->addRecipientsFromEntry(false);
        m_pRecipPanel->clearEntry();
    }
    if(!isRecipExists() && !isBodyEmpty() && m_Mode == NewMessageMode)
    {
        showNoRecipPopup();
        return;
    }
    else
    {
        saveDraftMsg();
        getParent().pop();
    }
}

void Conversation::onHwMoreButtonClicked()
{
    MSG_LOG("");
    if(m_Mode == ConversationMode && m_pConvList->getMode() == ConvList::NormalMode)
        showMainPopup();
}

void Conversation::onNaviOkButtonClicked()
{
    MSG_LOG("");
    if(m_Mode == ConversationMode && m_pConvList->getMode() == ConvList::SelectMode)
    {
        m_pConvList->deleteSelectedItems();
        m_pConvList->setMode(ConvList::NormalMode);
        updateNavibar();
    }
}

void Conversation::onNaviCenterButtonClicked()
{
    MSG_LOG("");
    if(m_pRecipPanel)
    {
        onNaviExpandButtonClicked();
    }
    else
    {
        MsgAddressListRef addressList = getAddressList();
        if(addressList && !addressList->isEmpty())
            recipientClickHandler(addressList->at(0).getAddress());
    }
}

void Conversation::onNaviExpandButtonClicked()
{
    MSG_LOG("");
    if(m_pRecipPanel)
    {
        bool isMbeInvisible = !m_pRecipPanel->isMbeVisible();
        m_pRecipPanel->showMbe(isMbeInvisible);
        getNaviBar().setDownButtonState(isMbeInvisible);
    }
}

void Conversation::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    //TODO: Handle other buttons
    switch(buttonId)
    {
        case NaviPrevButtonId:
            onHwBackButtonClicked();
            break;

        case NaviExpandButtonId:
            onNaviExpandButtonClicked();
            break;

        case NaviCenterButtonId:
            onNaviCenterButtonClicked();
            break;

        case NaviCancelButtonId:
            onHwBackButtonClicked();
            break;

        case NaviOkButtonId:
            onNaviOkButtonClicked();
            break;

        default:
            MSG_LOG_ERROR("Invalid buttonId: ", buttonId);
            break;
    }
}

void Conversation::onPopupDel(Evas_Object *popup, void *eventInfo)
{
    MSG_LOG("");
    m_pBody->setFocus(true);
}

void Conversation::onMsgSendErrorButtonClicked(Popup &popup, int buttonId)
{
    MSG_LOG("");
    m_pBody->setFocus(true);
    popup.destroy();
}

void Conversation::onNoRecipCancelButtonClicked(Popup &popup, int buttonId)
{
    m_pBody->setFocus(true);
    popup.destroy();
}

void Conversation::onNoRecipDiscardButtonClicked(Popup &popup, int buttonId)
{
    if(isDefferedCmd())
    {
        if(m_DefferedCmd.defaultCmd)
        {   // If we came from notification menu and unread threads will be more than one, we should to go back on thread list view
            if(m_DefferedCmd.defaultCmd->getDefaultType() == AppControlDefault::NotificationType && getMsgEngine().getStorage().getUnreadThreadCount() > 1)
            {
                getParent().pop();
            }
            else
            {   // Go to needed conversation
                MessageRef msg = getMsgEngine().getStorage().getMessage(m_DefferedCmd.defaultCmd->getMessageId());
                if(msg)
                    setThreadId(msg->getThreadId());
                if(m_DefferedCmd.defaultCmd->getDefaultType() == AppControlDefault::ReplyType)
                    m_pBody->setFocus(true);
            }
        }
        else if(m_DefferedCmd.composeCmd)
        {   // Go to needed conversation
            setThreadId(ThreadId());
            if(m_pRecipPanel)
                m_pRecipPanel->execCmd(m_DefferedCmd.composeCmd);
            if(m_pBody)
                m_pBody->execCmd(m_DefferedCmd.composeCmd);
        }
        resetDefferedCmd();
    }
    else
    {   // We will get here from conversation after tap on Back Button
        getParent().pop();
    }

    popup.destroy();
}

void Conversation::onDeleteItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_pConvList->setMode(ConvList::SelectMode);
    updateNavibar();
}

void Conversation::onAddRecipientsItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    setMode(NewMessageMode);
    m_ThreadId.reset();
}

void Conversation::onMakeVoiceItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    std::string address = static_cast<PopupAddressListItem&>(item).getAddress();
    item.getParent().destroy();
    VoiceCall::launch(address);
}

void Conversation::onCreateContactItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    std::string address = static_cast<PopupAddressListItem&>(item).getAddress();
    item.getParent().destroy();
    m_ContactEditor.launch(address, ContactEditor::CreateOp);
}

void Conversation::onUpdateContactItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    std::string address = static_cast<PopupAddressListItem&>(item).getAddress();
    item.getParent().destroy();
    m_ContactEditor.launch(address, ContactEditor::EditOp);
}

void Conversation::onRemoveItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_pRecipPanel->removeSelectedItem();
}

void Conversation::onEditItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    item.getParent().destroy();
    m_pRecipPanel->editSelectedItem();
}

void Conversation::onViewContactDetailsItemPressed(PopupListItem &item)
{
    MSG_LOG("");
    int personId = static_cast<PopupPersonIdListItem&>(item).getPersonId();
    item.getParent().destroy();
    ContactViewer::launch(personId);
}

void Conversation::onAllItemsDeleted(ConvList &list)
{
    MSG_LOG("");
    onHwBackButtonClicked();
}

void Conversation::onEditDraftMsg(MsgId id)
{
    MSG_LOG("");
    editDraftMsg(id);
}

void Conversation::onForwardMsg(MsgId id)
{
    MSG_LOG("");
    forwardMsg(id);
}

void Conversation::onSlideShow(MsgId id)
{
    MSG_LOG("");
    navigateToSlideShow(id);
}

void Conversation::onConvListItemChecked()
{
    getNaviBar().disabledButton(NaviOkButtonId, m_pConvList->getMessageCheckedCount() == 0);
}

void Conversation::onFileSelected(AttachPanel &panel, const AttachPanel::FileList &files)
{
    MSG_LOG("");
    m_pBody->addMedia(files);
}

void Conversation::onContactChanged()
{
    MSG_LOG("");
    contactChangedHandler();
}
