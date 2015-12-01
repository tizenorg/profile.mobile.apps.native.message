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

#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Logger.h"
#include "Message.h"
#include "MsgStorage.h"
#include "MessageSMS.h"
#include "MsgEngine.h"
#include "App.h"

using namespace Msg;

class TestMessageSender: public testing::Test
{
    protected:
        virtual void SetUp()
        {
            m_Engine.openService();
        }

        virtual void TearDown()
        {
            m_Engine.closeService();
        }
        MsgEngine m_Engine;
};

//____________________________________________________________________________//

TEST_F( TestMessageSender, SendSms )
{
    std::string number = "0941234567";

    MessageSMSRef sms = m_Engine.getStorage().createSms();

    sms->setText("Test");

    MsgAddress &msgAddr = sms->addAddress();
    msgAddr.setAddress(number);
    msgAddr.setRecipientType(MsgAddress::To);
    msgAddr.setAddressType(MsgAddress::Phone);

    ThreadId id;

    MsgTransport::ReturnType result = m_Engine.getTransport().sendMessage(sms, &id);

    testing::Test::RecordProperty("result", result);
    ASSERT_EQ(result, MsgTransport::ReturnSuccess);
}

//____________________________________________________________________________//

TEST_F( TestMessageSender, SendSmsToNotValidNumber )
{
    MessageSMSRef sms;
    ThreadId id;

    MsgTransport::ReturnType result = m_Engine.getTransport().sendMessage(sms, &id);

    testing::Test::RecordProperty("result", result);
    ASSERT_EQ(result, MsgTransport::ReturnNullPointer);
}

//____________________________________________________________________________//
