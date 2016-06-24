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
#include "ContactManager.h"
#include "LangUtils.h"
#include "TimeUtils.h"
#include "MessageMms.h"

#include <telephony_common.h>
#include <telephony_sim.h>

using namespace Msg;

std::string MessageDetailContent::getMsgDetailsPopupContent(App &app, MsgId msgId)
{
    return createMsgDetailsPopupText(app, msgId);
}

std::string MessageDetailContent::createMsgDetailsPopupText(App &app, MsgId msgId)
{
    std::string msgDetails;
    Message::Direction msgDirection = app.getMsgEngine().getStorage().getMessage(msgId)->getDirection();
    Message::Type msgType = app.getMsgEngine().getStorage().getMessage(msgId)->getType();
    Message::NetworkStatus msgStatus = app.getMsgEngine().getStorage().getMessage(msgId)->getNetworkStatus();
    ThreadId msgThreadId = app.getMsgEngine().getStorage().getMessage(msgId)->getThreadId();
    MsgStorage &msgStorage = app.getMsgEngine().getStorage();

    msgDetails += getMessageType(msgType);
    msgDetails += getContactsInfo(app, msgDirection, msgThreadId);
    msgDetails += getSentReceivedTime(msgStorage, msgDirection, msgId);

    if(msgDirection == Message::MD_Sent && msgType == Message::MT_SMS)
    {
        msgDetails.append("<br/>");
        msgDetails += makeDeliveryReportResult(app, msgStatus, msgType, msgThreadId, msgId);
        msgDetails += getSmsStatus(msgStatus);
    }
    else if(msgType == Message::MT_MMS_Noti)
    {
        msgDetails.append("<br/>");
        msgDetails += getMmsSubject(app, msgId);
        msgDetails += getMmsMessageSize(app, msgId);
        msgDetails += getMmsMessageExpired(app, msgId);
    }
    else if(msgType == Message::MT_MMS)
    {
        msgDetails.append("<br/>");
        msgDetails += getMmsSubject(app, msgId);
        msgDetails += getMmsMessageSize(app, msgId);

        if(msgDirection == Message::MD_Sent)
        {
            msgDetails.append("<br/>");
            msgDetails += makeDeliveryReportResult(app, msgStatus, msgType, msgThreadId, msgId);
            msgDetails += makeReadReportResult(app, msgId, msgThreadId, msgStatus);
        }
    }
    return msgDetails;
}

std::string MessageDetailContent::getMmsNotiConvListItemContent(App &app, MsgId msgId)
{
    std::string msgDetails;
    msgDetails += getMmsSubject(app, msgId);
    msgDetails += getMmsMessageSize(app, msgId);
    msgDetails += getMmsMessageExpired(app, msgId);
    return msgDetails;
}

std::string MessageDetailContent::getMessageType(Message::Type msgType)
{
    std::string msgDetails = msg("IDS_MSG_BODY_TYPE_C");
    msgDetails.append(" ");

    switch(msgType)
    {
    case Message::MT_SMS:
        msgDetails.append(msg("IDS_MSGF_OPT_TEXT_MESSAGE"));
        break;
    case Message::MT_MMS:
        msgDetails.append(msg("IDS_MSGF_BODY_MULTIMEDIA_MESSAGE"));
        break;
    case Message::MT_MMS_Noti:
        msgDetails.append(msg("IDS_MSGF_BODY_MULTIMEDIA_MESSAGE_NOTIFICATION"));
        break;
    default:
        MSG_LOG_WARN("Unknown message type");
    }
    msgDetails.append("<br/>");
    return msgDetails;
}

std::string MessageDetailContent::getContactsInfo(App &app, Message::Direction msgDirection, ThreadId msgThreadId)
{
    MsgAddressListRef addrList = app.getMsgEngine().getStorage().getAddressList(msgThreadId);
    std::string subscriberNumber = getSubscriberNumber();

    std::string contactsInfo = msg("IDS_MSGF_BODY_FLFROM");
    contactsInfo.append(" ");
    if(msgDirection == Message::MD_Sent)
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
        std::string address = addrList->at(i).getAddress();
        contactsInfo.append(makeDispAddress(app, address));
        if(i != addressListLength - 1)
            contactsInfo.append(", ");
    }
    contactsInfo.append("<br/>");

    if(msgDirection == Message::MD_Received)
    {
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

std::string MessageDetailContent::getSentReceivedTime(MsgStorage &msgStorage, Message::Direction msgDirection, MsgId msgId)
{
    std::string msgDetails;
    if(msgDirection == Message::MD_Sent)
        msgDetails.append(msg("IDS_MSG_BODY_TIME_SENT_C"));
    else if(msgDirection == Message::MD_Received)
        msgDetails.append(msg("IDS_MSG_BODY_TIME_RECEIVED_C"));

    msgDetails.append(" ");
    time_t time = msgStorage.getMessage(msgId)->getTime();
    msgDetails.append(TimeUtils::makeDateTimeString(time));
    return msgDetails;
}

std::string MessageDetailContent::makeDeliveryReportResult(App &app, Message::NetworkStatus msgStatus, Message::Type msgType, ThreadId msgThreadId, MsgId msgId)
{
    std::string deliverText;

    bool deliverFlag = app.getMsgEngine().getStorage().isDeliverReportChecked(msgId);
    if(deliverFlag)
    {
        MsgReportListRef reportList = app.getMsgEngine().getStorage().getMsgReportList(msgId);
        int size = reportList->getLength();
        bool isDelivReportExists = false;

        for(int i = 0; i < size; i++)
        {
            const MsgReport &report = reportList->at(i);
            if(report.getType() == MsgReport::TypeDelivery)
            {
                isDelivReportExists = true;

                deliverText.append("<br/>");
                deliverText.append(makeDispAddress(app, report.getAddress()));
                deliverText.append(" - ");

                if(report.getDeliveryStatus() == MsgReport::StatusSuccess)
                {
                    deliverText.append(msg("IDS_MSGF_BODY_RECEIVED"));
                    deliverText.append(" (");

                    time_t time = report.getTime();
                    if(MsgUtils::isMms(msgType))
                        deliverText.append(TimeUtils::makeMmsReportTimeString(time));
                    else
                        deliverText.append(TimeUtils::makeSmsReportTimeString(time));

                    deliverText.append(")");
                }
                else if(report.getDeliveryStatus() == MsgReport::StatusExpired)
                {
                    deliverText.append(msg("IDS_MSGF_BODY_EXPIRED"));
                }
                else if(report.getDeliveryStatus() == MsgReport::StatusRejected)
                {
                    deliverText.append(msg("IDS_MSGF_POP_REJECTED"));
                }
                else if(report.getDeliveryStatus() == MsgReport::StatusNone)
                {
                    deliverText.append(msg("IDS_MSGF_BODY_REQUESTED"));
                }
                else
                {
                    deliverText.append(msg("IDS_MSGF_BODY_UNKNOWN"));
                }
            }
        }
        if(!isDelivReportExists)
            deliverText.append(msg("IDS_MSGF_BODY_UNKNOWN"));
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
    std::string subscriberNumber;
    telephony_handle_list_s telHandleList;
    int err = telephony_init(&telHandleList);

    if(err == TELEPHONY_ERROR_NONE)
    {
        if(telHandleList.count != 0)
        {
            char *tmpSubscriberNumber = nullptr;
            telephony_sim_get_subscriber_number(telHandleList.handle[0], &tmpSubscriberNumber);
            if(tmpSubscriberNumber)
            {
                subscriberNumber = tmpSubscriberNumber;
                free(tmpSubscriberNumber);
            }
        }
        telephony_deinit(&telHandleList);
    }

    return !subscriberNumber.empty() ? subscriberNumber : msg("IDS_MSGF_BODY_UNKNOWN");

}

std::string MessageDetailContent::getSmsStatus(Message::NetworkStatus msgStatus)
{
    std::string status = msg("IDS_MSG_BODY_STATUS_C");
    status.append(" ");

    switch(msgStatus)
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
            MSG_LOG("Indifference type of network status ", msgStatus);
            break;
    }

    return status;
}

std::string MessageDetailContent::getMmsSubject(App &app, MsgId msgId)
{
    std::string msgDetails = msg("IDS_MSGF_OPT_SUBJECT_COLON");
    msgDetails.append(" ");

    std::string subject = app.getMsgEngine().getStorage().getMessage(msgId)->getSubject();
    if(subject.empty())
        msgDetails.append(msg("IDS_MSGF_BODY_NO_SUBJECT"));
    else
        msgDetails += subject;

    msgDetails.append("<br/>");
    return msgDetails;
}

std::string MessageDetailContent::getMmsMessageSize(App &app, MsgId msgId)
{
    std::string msgDetails = msg("IDS_MSGF_BODY_MESSAGE_SIZE");
    msgDetails.append(": ");

    int msgSize = app.getMsgEngine().getStorage().getMessage(msgId)->getSize();
    if(msgSize < 1024)/*checking if the MMS size is less than 1 KB. In this case, we need to show it as 1KB.*/
        msgSize = 1;
    else if(msgSize%1024 != 0)/*For the case of msgSize > 1024 bytes, else part is called to check if rounding off is required or not.*/
        msgSize = round(msgSize / 1024);

    msgDetails.append(std::to_string(msgSize));
    msgDetails.append(msg("IDS_MSGF_BODY_MSGSIZE_KB"));

    msgDetails.append("<br/>");
    return msgDetails;
}

std::string MessageDetailContent::getMmsMessageExpired(App &app, MsgId msgId)
{
    std::string msgDetails = msg("IDS_MSGF_BODY_EXPIRED");
    msgDetails.append(": ");

    MessageMmsRef mms = std::dynamic_pointer_cast<MessageMms>(app.getMsgEngine().getStorage().getMessage(msgId));
    if(mms)
    {
        time_t msgExpired = mms->getExpired();
        msgDetails.append(TimeUtils::makeDateTimeString(msgExpired));
        msgDetails.append("<br/>");
    }
    return msgDetails;
}

std::string MessageDetailContent::makeReadReportResult(App &app, MsgId msgId, ThreadId msgThreadId, Message::NetworkStatus msgStatus)
{
    std::string readReport;
    bool readFlag = app.getMsgEngine().getStorage().isReadReportChecked(msgId);
    if(readFlag)
    {
        MsgReportListRef reportList = app.getMsgEngine().getStorage().getMsgReportList(msgId);

        int size = reportList->getLength();
        bool isReadReportExists = false;

        for(int i = 0; i < size; i++)
        {
            const MsgReport &report = reportList->at(i);
            if(report.getType() == MsgReport::TypeRead)
            {
                isReadReportExists = true;

                readReport.append("<br/>");
                readReport.append(makeDispAddress(app, report.getAddress()));
                readReport.append(" - ");

                if(report.getReadStatus() == MsgReport::ReadStatusIsRead)
                {
                    readReport.append(msg("IDS_MSGF_BODY_MMSREADREPLYMSGREAD"));
                    readReport.append(" (");
                    time_t time = report.getTime();
                    readReport.append(TimeUtils::makeDateTimeString(time));
                    readReport.append(")");
                }
                else if(report.getReadStatus() == MsgReport::ReadStatusIsDeleted)
                {
                    readReport.append(msg("IDS_MSG_POP_DELETED"));
                }
                else if(report.getReadStatus() == MsgReport::ReadStatusNone)
                {
                    readReport.append(msg("IDS_MSGF_BODY_REQUESTED"));
                }
                else
                {
                    readReport.append(msg("IDS_MSGF_BODY_UNKNOWN"));
                }
            }
        }

        if(!isReadReportExists)
            readReport.append(msg("IDS_MSGF_BODY_UNKNOWN"));
    }
    else
    {
        readReport.append(msg("IDS_MSGC_BODY_NOT_REQUESTED"));
    }

    return msgArgs("IDS_MSG_BODY_READ_REPORT_C_PS", readReport.c_str());
}

std::string MessageDetailContent::makeDispAddress(App &app, const std::string &address)
{
    std::string res;
    ContactAddressRef rec = app.getContactManager().getContactAddress(address);

    std::string dispName;
    if(rec)
        dispName = rec->getDispName();

    res = dispName;

    if(!res.empty())
        res += " (";

    res += address;
    if(!dispName.empty())
        res += ')';

    if(res.empty())
        res += msg("IDS_MSGF_BODY_UNKNOWN");

    return res;
}
