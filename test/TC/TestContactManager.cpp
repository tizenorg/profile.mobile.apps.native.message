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
#include "ContactUtils.h"
#include "BaseController.h"
#include "Logger.h"

using namespace Msg;

class TestContactManager: public testing::Test
{
    protected:
        virtual void SetUp()
        {
            m_ContactId = -100; //dummy id
        }

        virtual void TearDown()
        {
            if (m_ContactId != -100)
            {
                Msg::Test::ContactUtils::getInst().removeContact(m_ContactId);
            }
        }
        int m_ContactId;
        ContactManager m_ContactManager;
};

//____________________________________________________________________________//

TEST_F( TestContactManager, FindNameSasha )
{
    const std::string strNumber = "0936403503";
    const std::string strName = "Sasha";

    m_ContactId = Msg::Test::ContactUtils::getInst().createContact(strName, strNumber);
    ContactPersonNumber item = m_ContactManager.getContactPerson(strNumber);
    std::string result = item.getDispName();

    testing::Test::RecordProperty("Number", strNumber.c_str());
    testing::Test::RecordProperty("Name", strName.c_str());

    ASSERT_EQ(result, strName);
}

//____________________________________________________________________________//

TEST_F( TestContactManager, FindNameVova )
{
    const std::string strNumber = "0501234567";
    const std::string strName = "Vova";

    m_ContactId = Msg::Test::ContactUtils::getInst().createContact(strName, strNumber);

    ContactPersonNumber item = m_ContactManager.getContactPerson(strNumber);
    std::string result = item.getDispName();

    testing::Test::RecordProperty("Number", strNumber.c_str());
    testing::Test::RecordProperty("Name", strName.c_str());

    ASSERT_EQ(result, strName);
}

//____________________________________________________________________________//

TEST_F( TestContactManager, TestGetNameById )
{
    const std::string strNumber = "0971234567";
    const std::string strName = "Test1";

    m_ContactId = Msg::Test::ContactUtils::getInst().createContact(strName, strNumber);

    ContactPersonNumber item = m_ContactManager.getContactPerson(strNumber);
    std::string result1 = item.getDispName();

    std::string result2 = Msg::Test::ContactUtils::getInst().getNameById(m_ContactId);

    ASSERT_EQ(result1, result2);
}

//____________________________________________________________________________//

TEST_F( TestContactManager, TestRenameContact )
{
    const std::string strNumber = "0971234567";
    const std::string strName = "Test1";
    const std::string newName = "Test2";

    m_ContactId = Msg::Test::ContactUtils::getInst().createContact(strName, strNumber);
    Msg::Test::ContactUtils::getInst().renameContact(m_ContactId, newName);

    std::string result = Msg::Test::ContactUtils::getInst().getNameById(m_ContactId);

    ASSERT_EQ(newName, result);
}

//____________________________________________________________________________//

