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

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "MsgUtils.h"

BOOST_AUTO_TEST_SUITE(TestMsgUtils)

BOOST_AUTO_TEST_CASE(isValidNumberTest)
{
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+3804534332"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+38(045)34332"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+380453-43-32"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+380453 43 32"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+380453*43*32"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+380453- *43-32"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("3804534332"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+----------"), false);

    //Min len 3 digit
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+32"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("3-2"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("332"), true);

    //Max len 15
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+380453433212115"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+38(045)343-32-12-115"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("380453433212115"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("3804534332121156"), false);

    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+3804a534332"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("+3804[53]4332"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidNumber("Contact Name"), false);
}

BOOST_AUTO_TEST_CASE(isValidEmailTest)
{
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("prettyandsimple@example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("very.common@example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("disposable.style.email.with+symbol@example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("other.email-with-dash@example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("\"much.more unusual\"@example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("\"very.unusual.@.unusual.com\"@example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("\"very.(),:;<>[]\\\".VERY.\\\"very@\\\\ \\\"very\\\".unusual\"@strange.example.com"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("admin@mailserver1"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("#!$%&'*+-/=?^_`{}|~@example.org"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("\"()<>[]:,;@\\\"!#$%&'*+-/=?^_`{}| ~.a\"@example.org"), true);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("\" \"@example.org"), true);

    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("Abc.example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("A@b@c@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("a\"b(c)d,e:f;g<h>i[j\\k]l@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("just\"not\"right@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("this is\"not\\allowed@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("this\\ still\\\"not\\\\allowed@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("john..doe@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("john.doe@example..com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail(" leadingspace@example.com"), false);
    BOOST_CHECK_EQUAL(Msg::MsgUtils::isValidEmail("trailing space@example.com"), false);
}

BOOST_AUTO_TEST_SUITE_END()
