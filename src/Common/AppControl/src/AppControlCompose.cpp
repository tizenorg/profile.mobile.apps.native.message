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
 */

#include "AppControlCompose.h"

using namespace Msg;

AppControlCompose::AppControlCompose(const std::string &opMsg)
    : AppControlCommand(opMsg, OpCompose)
    , m_MessageType(UnknownType)
{
}

AppControlCompose::~AppControlCompose()
{

}

const AppControlCompose::RecipientList &AppControlCompose::getRecipientList() const
{
    //TODO: implementation
    return m_RecipientList;
}

AppControlCompose::MessageType AppControlCompose::getMessageType() const
{
    return m_MessageType;
}
const std::string AppControlCompose::getMessageText() const
{
    //TODO: implementation
    return "";
}
const std::string AppControlCompose::getMessageSubject() const
{
    //TODO: implementation
    return "";
}
const AppControlCompose::FileList &AppControlCompose::getFileList() const
{
    //TODO: implementation
    return m_FileList;
}
