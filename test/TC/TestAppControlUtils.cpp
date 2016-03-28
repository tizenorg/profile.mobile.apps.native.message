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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AppControlUtils.h"

using namespace Msg;

class TestAppControlUtils: public testing::Test
{
    protected:
        virtual void SetUp()
        {
            app_control_create(&m_handle);
        }

        virtual void TearDown()
        {
            app_control_destroy(m_handle);
        }
        app_control_h m_handle;
};

TEST_F( TestAppControlUtils, GetExtraData)
{
    const std::string stringToBeFound("test-string");
    app_control_add_extra_data(m_handle, APP_CONTROL_DATA_PATH, stringToBeFound.c_str());
    ASSERT_EQ(stringToBeFound, AppControlUtils::getExtraData(m_handle, APP_CONTROL_DATA_PATH));

    const std::string stringToBeNotFound("not-written-string");
    ASSERT_NE(stringToBeNotFound, AppControlUtils::getExtraData(m_handle, APP_CONTROL_DATA_PATH));
}

TEST_F( TestAppControlUtils, GetExtraDataArray)
{
    const size_t arrSize = 4;
    char* arr[] = {"hello", "world", "asdf", "qwerty"};
    app_control_add_extra_data_array(m_handle, APP_CONTROL_DATA_PATH, arr, arrSize);

    std::list<std::string> gotList;
    AppControlUtils::getExtraDataArray(m_handle, APP_CONTROL_DATA_PATH, gotList);

    int i=0;
    for(auto num: gotList)
    {
        std::string s(arr[i]);
        ASSERT_EQ(s, num);
        ++i;
    }
}

TEST_F( TestAppControlUtils, getExtraDataInt)
{
    const char* numsStr = "100";
    app_control_add_extra_data(m_handle, APP_CONTROL_DATA_SELECTED, numsStr);

    int number = AppControlUtils::getExtraDataInt(m_handle, APP_CONTROL_DATA_SELECTED);

    std::string s(numsStr);
    ASSERT_EQ(s, std::to_string(number));
}

TEST_F( TestAppControlUtils, GetExtraDataIntArray)
{
    const std::size_t arrSize = 4;
    const char* numsStrList[] = {"1", "2", "3", "4"};
    app_control_add_extra_data_array(m_handle, APP_CONTROL_DATA_SELECTED, numsStrList, arrSize);

    std::list<int> gotNums;
    AppControlUtils::getExtraDataIntArray(m_handle, APP_CONTROL_DATA_SELECTED, gotNums);
    int i=0;
    for(auto num: gotNums)
    {
        std::string s(numsStrList[i]);
        ASSERT_EQ(s, std::to_string(num));
        ++i;
    }
}
