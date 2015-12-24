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

#include "AppControlCommand.h"

using namespace Msg;

AppControlCommand::AppControlCommand(const std::string &opMsg, OperationType type)
    : m_OperationMsg(opMsg)
    , m_Type(type)
{
}

AppControlCommand::~AppControlCommand()
{
}

const std::string &AppControlCommand::getOperationMsg() const
{
    return m_OperationMsg;
}

AppControlCommand::OperationType AppControlCommand::getOperationType() const
{
    return m_Type;
}

bool AppControlCommand::initialize(app_control_h handle)
{
    return false;
}

