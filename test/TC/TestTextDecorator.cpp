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
#include "TextDecorator.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TestTextDecorator, customStyleTest)
{
    std::string text("a text to format");
    Msg::TextStyle styleToApply(50, "#00ff00ff", Msg::TextAlign::Center);
    std::string result = Msg::TextDecorator::make(text, styleToApply);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    testing::Test::RecordProperty("result", result.c_str());

    std::size_t foundPos = result.find("<font_size=50");
    ASSERT_NE(foundPos, std::string::npos);

    foundPos = result.find("<color=#00ff00ff");
    ASSERT_NE(foundPos, std::string::npos);

    foundPos = result.find("<align=center");
    ASSERT_NE(foundPos, std::string::npos);
}

TEST( TestTextDecorator, defaultFormatTest )
{
    std::string text("a text to format");
    Msg::TextStyle defaultStyle;
    std::string result = Msg::TextDecorator::make(text, defaultStyle);
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    testing::Test::RecordProperty("result", result.c_str());

    std::size_t foundPos = result.find("<font_size=28");
    ASSERT_NE(foundPos, std::string::npos);

    foundPos = result.find("<color=#000000ff");
    ASSERT_NE(foundPos, std::string::npos);

    foundPos = result.find("<align=left");
    ASSERT_EQ(foundPos, std::string::npos);
}
