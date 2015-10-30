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

#include <string>
#include "TextDecorator.h"
#include <boost/test/unit_test.hpp>
#include <functional>

BOOST_AUTO_TEST_SUITE(TestTextDecorator)

BOOST_AUTO_TEST_CASE( CustomFormatTest )
{
    std::string text("a text to format");
    Msg::TextStyle styleToApply(50, "#00ff00ff", Msg::TextAlign::Center);
    std::string result = Msg::TextDecorator::make(text, styleToApply);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    BOOST_TEST_MESSAGE("result string is " << result);

    std::size_t foundPos = result.find("<font_size=50");
    BOOST_CHECK_PREDICATE(std::not_equal_to<std::size_t>(), (foundPos)(std::string::npos));

    foundPos = result.find("<color=#00ff00ff");
    BOOST_CHECK_PREDICATE(std::not_equal_to<std::size_t>(), (foundPos)(std::string::npos));

    foundPos = result.find("<align=center");
    BOOST_CHECK_PREDICATE(std::not_equal_to<std::size_t>(), (foundPos)(std::string::npos));
}

BOOST_AUTO_TEST_CASE( DefaultFormatTest )
{
    std::string text("a text to format");
    Msg::TextStyle defaultStyle;
    std::string result = Msg::TextDecorator::make(text, defaultStyle);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    BOOST_TEST_MESSAGE("result string is " << result);

    std::size_t foundPos = result.find("<font_size=28");
    BOOST_CHECK_PREDICATE(std::not_equal_to<std::size_t>(), (foundPos)(std::string::npos));

    foundPos = result.find("<color=#000000ff");
    BOOST_CHECK_PREDICATE(std::not_equal_to<std::size_t>(), (foundPos)(std::string::npos));

    foundPos = result.find("<align=left");
    BOOST_CHECK_PREDICATE(std::not_equal_to<std::size_t>(), (foundPos)(std::string::npos));
}

BOOST_AUTO_TEST_SUITE_END()
