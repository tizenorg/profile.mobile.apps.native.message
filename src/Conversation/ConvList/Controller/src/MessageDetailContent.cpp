/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "MessageDetailContent.h"
#include "ThumbnailMaker.h"
#include <telephony_common.h>
#include <telephony_sim.h>
#include "ContactManager.h"
#include "LangUtils.h"

using namespace Msg;

MessageDetailContent::MessageDetailContent(App &app, MsgId &msgId, Message::NetworkStatus &status, Message::Direction &direction)
    : m_App(app)
    , m_MsgId(msgId)
    , m_ThreadId(app.getMsgEngine().getStorage().getMessage(msgId)->getThreadId())
    , m_Status(status)
    , m_Type(app.getMsgEngine().getStorage().getMessage(msgId)->getType())
    , m_Direction(direction)
{
    createMsgDetailsText();
}

MessageDetailContent::~MessageDetailContent()
{
}

std::string MessageDetailContent::getMsgDetailContent() const
{
    return m_MsgDetails;
}

void MessageDetailContent::createMsgDetailsText()
{
    m_MsgDetails += getMessageType();
    m_MsgDetails += getContactsInfo();
    m_MsgDetails += getSentReceivedTime();

    if(m_Direction == Message::MD_Sent && m_Type == Message::MT_SMS)
    {
        m_MsgDetails.append("<br/>");
        m_MsgDetails += makeReportResult();
        m_MsgDetails += getSmsStatus();
    }
    else if(m_Type == Message::MT_MMS)
    {
        m_MsgDetails.append("<br/>");
        m_MsgDetails += getMmsSubject();
        m_MsgDetails += getMmsMessageSize();

        if(m_Direction == Message::MD_Sent)
        {
            m_MsgDetails.append("<br/>");
            m_MsgDetails += makeReportResult();
            m_MsgDetails += makeReadReportResult();
        }
    }
}

std::string MessageDetailContent::getMessageType()
{
    std::string msgDetails = msg("IDS_MSG_BODY_TYPE_C");
    msgDetails.append(" ");

    if(m_Type == Message::MT_SMS)
        msgDetails.append(msg("IDS_MSGF_OPT_TEXT_MESSAGE"));
    else if(m_Type == Message::MT_MMS)
        msgDetails.append(msg("IDS_MSGF_BODY_MULTIMEDIA_MESSAGE"));

    msgDetails.append("<br/>");
    return msgDetails;
}

std::string MessageDetailContent::getContactsInfo()
{
    MsgAddressListRef addrList = m_App.getMsgEngine().getStorage().getAddressList(m_ThreadId);
    std::string subscriberNumber = getSubscriberNumber();

    std::string contactsInfo = msg("IDS_MSGF_BODY_FLFROM");
    contactsInfo.append(" ");
    if(m_Direction == Message::MD_Sent)
    {
        contactsInfo.append(msg("IDS_MSGF_BODY_ME"));
        contactsInfo.append(" (");
        contactsInfo.append(subscriberNumber);
        contactsInfo.append(")");
        contactsInfo.append("<br/>");
        contactsInfo.append(msg("IDS_MSGF_BODY_TO_C"));
        contactsInfo.append(" ");
    }

    int addressListLength = addrList->getLength();
    for(int i = 0; i < addressListLength; ++i)
    {
        std::string number = addrList->at(i).getAddress();
        ContactPersonNumberRef contact = m_App.getContactManager().getContactPersonNumber(number);
        if(contact)
        {
            contactsInfo.append(contact->getDispName().c_str());
            contactsInfo.append(" (");
            contactsInfo.append(number);
            contactsInfo.append(")");
        }
        else
        {
            if(number.empty())
                contactsInfo.append(msg("IDS_MSGF_BODY_UNKNOWN"));
            else
                contactsInfo.append(number);
        }
        if(i != addressListLength - 1)
            contactsInfo.append(", ");
    }
    contactsInfo.append("<br/>");

    if ((m_Direction == Message::MD_Received))  {
        contactsInfo.append(msg("IDS_MSGF_BODY_TO_C"));
        contactsInfo.append(" ");
        contactsInfo.append(msg("IDS_MSGF_BODY_ME"));
        contactsInfo.append(" (");
        contactsInfo.append(subscriberNumber);
        contactsInfo.append(")");
        contactsInfo.append("<br/>");
    }

    return contactsInfo;
}

std::string MessageDetailContent::getSentReceivedTime()
{
    std::string msgDetails;
    if(m_Direction == Message::MD_Sent)
        msgDetails.append(msg("IDS_MSG_BODY_TIME_SENT_C"));
    else if(m_Direction == Message::MD_Received)
        msgDetails.append(msg("IDS_MSG_BODY_TIME_RECEIVED_C"));

    msgDetails.append(" 12:00 12/12/2012"); //TODO: add time when it will be implement
    return msgDetails;
}

std::string MessageDetailContent::makeReportResult()
{
    std::string deliverText;

    if(m_Status == Message::NS_Send_Success || m_Status == Message::NS_Deliver_Success)
    {
        MsgReportListRef reportList = m_App.getMsgEngine().getStorage().getMsgReportList(m_MsgId);
        MsgAddressListRef addrList = m_App.getMsgEngine().getStorage().getAddressList(m_ThreadId);

        int size = addrList->getLength();
        for(int i = 0; i < size; i++)
        {
            if(reportList->getLength() != 0)
            {
                deliverText.append("<br/>");
                deliverText.append(addrList->at(i).getAddress());
                deliverText.append(" - ");
                if(reportList->at(i).getType() == MsgReport::TypeDelivery)
                {
                    if(reportList->at(i).getDeliveryStatus() == MsgReport::StatusSuccess)
                    {
                        deliverText.append(msg("IDS_MSGF_BODY_RECEIVED"));
                        deliverText.append(" (12:40 friday 26.12.2016)"); // TODO: when time locale will be implement
                    }
                    else if(reportList->at(i).getDeliveryStatus() == MsgReport::StatusExpired)
                    {
                        deliverText.append(msg("IDS_MSGF_BODY_EXPIRED"));
                    }
                    else if(reportList->at(i).getDeliveryStatus() == MsgReport::StatusRejected)
                    {
                        deliverText.append(msg("IDS_MSGF_POP_REJECTED"));
                    }
                    else
                    {
                        if(m_Type == Message::MT_MMS)
                            deliverText.append(msg("IDS_MSGF_BODY_UNKNOWN"));
                        else
                            deliverText.append(msg("IDS_MSGF_BODY_REQUESTED"));
                    }
                }
            }
            else
            {
                deliverText.append("<br/>");
                deliverText.append(addrList->at(i).getAddress());
                deliverText.append(" - ");
                deliverText.append(msg("IDS_MSGC_BODY_NOT_REQUESTED"));
            }
        }
    }
    else
    {
        deliverText.append(msg("IDS_MSGC_BODY_NOT_REQUESTED"));
    }

    deliverText.append("<br/>");
    return msgArgs("IDS_MSG_BODY_DELIVERY_REPORT_C_PS", deliverText.c_str());
}

std::string MessageDetailContent::getSubscriberNumber()
{
    char *subscriberNumber = nullptr;
    telephony_handle_list_s telHandleList;
    int err = telephony_init(&telHandleList);

    if(err == TELEPHONY_ERROR_NONE)
    {
        if(telHandleList.count != 0)
        {
            err = telephony_sim_get_subscriber_number(telHandleList.handle[0], &subscriberNumber);
            if(err != TELEPHONY_ERROR_NONE)
            {
                if(subscriberNumber)
                    free(subscriberNumber);
                MSG_LOG_ERROR("Cannot get self phone number, err = %d", err);
            }
        }
        telephony_deinit(&telHandleList);
    }

    if(subscriberNumber)
        return subscriberNumber;

    return msg("IDS_MSGF_BODY_UNKNOWN");
}

std::string MessageDetailContent::getSmsStatus()
{
    std::string status = msg("IDS_MSG_BODY_STATUS_C");
    status.append(" ");

    switch(m_Status)
    {
        case Message::NS_Sending:
            status.append(msg("IDS_MSG_BODY_SENDING_ING_M_STATUS_ABB"));
            break;
        case Message::NS_Send_Success:
        case Message::NS_Deliver_Success:
            status.append(msg("IDS_MSG_BODY_SENT_M_STATUS"));
            break;
        case Message::NS_Send_Pending:
            status.append(msg("IDS_MSG_BODY_WAITING_TO_SEND_ING_M_STATUS"));
            break;
        case Message::NS_Send_Fail:
        case Message::NS_Deliver_Fail:
            status.append(msg("IDS_MSG_BODY_SENDING_FAILED_M_STATUS"));
            break;
        default:
            MSG_LOG("Indifference type of network status ", m_Status);
            break;
    }

    return status;
}

std::string MessageDetailContent::getMmsSubject()
{
    std::string msgDetails = msg("IDS_MSGF_OPT_SUBJECT_COLON");
    msgDetails.append(" ");

    std::string subject = m_App.getMsgEngine().getStorage().getMessage(m_MsgId)->getSubject();
    if(subject.empty())
        msgDetails.append(msg("IDS_MSGF_BODY_NO_SUBJECT"));
    else
        msgDetails += subject;

    msgDetails.append("<br/>");
    return msgDetails;
}

std::string MessageDetailContent::getMmsMessageSize()
{
    std::string msgDetails = msg("IDS_MSGF_BODY_MESSAGE_SIZE");
    msgDetails.append(": ");

    int msgSize = m_App.getMsgEngine().getStorage().getMessage(m_MsgId)->getMessageSize();
    if(msgSize < 1024)/*checking if the MMS size is less than 1 KB. In this case, we need to show it as 1KB.*/
        msgSize = 1;
    else if(msgSize%1024 != 0)/*For the case of msgSize > 1024 bytes, else part is called to check if rounding off is required or not.*/
        msgSize = round(msgSize / 1024);

    msgDetails.append(std::to_string(msgSize));
    msgDetails.append(msg("IDS_MSGF_BODY_MSGSIZE_KB"));

    return msgDetails;
}

std::string MessageDetailContent::makeReadReportResult()
{
    std::string readReport;
    bool readFlag = m_App.getMsgEngine().getStorage().isReadReportChecked(m_MsgId);
    MSG_LOG("readFlag = ", readFlag);
    if((m_Status == Message::NS_Send_Success || m_Status == Message::NS_Deliver_Success) && readFlag)
    {
        MsgReportListRef reportList = m_App.getMsgEngine().getStorage().getMsgReportList(m_MsgId);
        MsgAddressListRef addrList = m_App.getMsgEngine().getStorage().getAddressList(m_ThreadId);

        int size = addrList->getLength();
        for(int i = 0; i < size; i++)
        {
            readReport.append("<br/>");
            readReport.append(addrList->at(i).getAddress());
            readReport.append(" - ");
            if(reportList->at(i).getType() == MsgReport::TypeDelivery)
            {
                if(reportList->at(i).getReadStatus() == MsgReport::ReadStatusIsRead)
                {
                    readReport.append(msg("IDS_MSGF_BODY_MMSREADREPLYMSGREAD"));
                    readReport.append(" (12:40 friday 26.12.2016)"); // TODO: when time locale will be implement
                }
                else if(reportList->at(i).getReadStatus() == MsgReport::ReadStatusIsDeleted)
                {
                    readReport.append(msg("IDS_MSG_POP_DELETED"));
                }
                else if(reportList->at(i).getReadStatus() == MsgReport::ReadStatusNone)
                {
                    readReport.append(msg("IDS_MSGF_BODY_REQUESTED"));
                }
            }
        }
    }
    else
    {
        readReport.append(msg("IDS_MSGC_BODY_NOT_REQUESTED"));
    }

    return msgArgs("IDS_MSG_BODY_READ_REPORT_C_PS", readReport.c_str());
}
