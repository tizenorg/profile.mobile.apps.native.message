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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "MsgUtils.h"


TEST( TestMsgUtils, isValidNumberTest)
{
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+3804534332"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+38(045)34332"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+380453-43-32"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+380453 43 32"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+380453*43*32"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+380453- *43-32"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("3804534332"));
	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("+----------"));

	//Min len 3 digit
	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("+32"));
	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("3-2"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("332"));

	//Max len 15
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+380453433212115"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("+38(045)343-32-12-115"));
	ASSERT_TRUE(Msg::MsgUtils::isValidNumber("380453433212115"));
	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("3804534332121156"));

	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("+3804a534332"));
	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("+3804[53]4332"));
	ASSERT_FALSE(Msg::MsgUtils::isValidNumber("Contact Name"));
}

TEST( TestMsgUtils, isValidEmailTest)
{
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("prettyandsimple@example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("very.common@example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("disposable.style.email.with+symbol@example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("other.email-with-dash@example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("\"much.more unusual\"@example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("\"very.unusual.@.unusual.com\"@example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("\"very.(),:;<>[]\\\".VERY.\\\"very@\\\\ \\\"very\\\".unusual\"@strange.example.com"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("admin@mailserver1"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("#!$%&'*+-/=?^_`{}|~@example.org"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("\"()<>[]:,;@\\\"!#$%&'*+-/=?^_`{}| ~.a\"@example.org"));
	ASSERT_TRUE(Msg::MsgUtils::isValidEmail("\" \"@example.org"));

	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("Abc.example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("A@b@c@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("a\"b(c)d,e:f;g<h>i[j\\k]l@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("just\"not\"right@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("this is\"not\\allowed@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("this\\ still\\\"not\\\\allowed@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("john..doe@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("john.doe@example..com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail(" leadingspace@example.com"));
	ASSERT_FALSE(Msg::MsgUtils::isValidEmail("trailing space@example.com"));
}

TEST( TestMsgUtils, makeNormalizedNumberTest)
{
    ASSERT_EQ(Msg::MsgUtils::makeNormalizedNumber("+38(045)34332"), "+3804534332");
    ASSERT_EQ(Msg::MsgUtils::makeNormalizedNumber("+380453-43-32"), "+3804534332");
    ASSERT_EQ(Msg::MsgUtils::makeNormalizedNumber("+380453 43 32"), "+3804534332");
    ASSERT_EQ(Msg::MsgUtils::makeNormalizedNumber("+380453*43*32"), "+380453*43*32");
    ASSERT_EQ(Msg::MsgUtils::makeNormalizedNumber("+380453- *43-32"), "+380453*4332");
    ASSERT_EQ(Msg::MsgUtils::makeNormalizedNumber("+38(045)343-32-12-115"), "+380453433212115");
}
