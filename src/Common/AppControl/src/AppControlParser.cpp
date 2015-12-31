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

#include "Logger.h"
#include "AppControlParser.h"
#include "AppControlCommand.h"
#include "AppControlCommandDefault.h"
#include "AppControlCompose.h"

#include <app_control.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>

using namespace Msg;

namespace
{
    typedef std::unordered_map<std::string, AppControlCommand::OperationType> OperationMap;

    const OperationMap operationMap =
    {
        {APP_CONTROL_OPERATION_DEFAULT, AppControlCommand::OpDefault},
        {APP_CONTROL_OPERATION_MAIN, AppControlCommand::OpDefault},
        {APP_CONTROL_OPERATION_COMPOSE, AppControlCommand::OpComposeFamily},
        {APP_CONTROL_OPERATION_SHARE, AppControlCommand::OpComposeFamily},
        {APP_CONTROL_OPERATION_MULTI_SHARE, AppControlCommand::OpComposeFamily},
        {APP_CONTROL_OPERATION_SHARE_TEXT, AppControlCommand::OpComposeFamily}
    };

    AppControlCommand::OperationType getOperation(const char *op)
    {
        auto it = operationMap.find(op);
        return it != operationMap.end() ? it->second : AppControlCommand::OpUnknown;
    };
}

AppControlCommandRef AppControlParser::parse(app_control_h handle)
{
    AppControlCommandRef cmd;
    char *opStr = nullptr;

    app_control_get_operation(handle, &opStr);

    if(!opStr)
    {
        MSG_LOG_WARN("app_control operation is null");
        return cmd;
    }

    AppControlCommand::OperationType opType = getOperation(opStr);

    switch(opType)
    {
        //TODO: Create classes for all operations
        case AppControlCommand::OpDefault:
            cmd = std::make_shared<AppControlCommandDefault>(opStr);
            break;

        case AppControlCommand::OpComposeFamily:
            cmd = std::make_shared<AppControlCompose>(opStr, handle);
            break;

        // TODO: impl for other command types

        default:
        case AppControlCommand::OpUnknown:
            cmd = std::make_shared<AppControlCommand>(opStr, opType);
            break;
    }

    free(opStr);
    return cmd;
}
