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

#include "NaviFrameController.h"
#include "FrameController.h"
#include "App.h"
#include "MsgThread.h"
#include "Conversation.h"
#include "Logger.h"
#include "AppControlCommandDefault.h"
#include "AppControlCompose.h"

#include <memory>
#include <notification.h>

using namespace Msg;

NaviFrameController::NaviFrameController(App &app)
    : ViewController(app)
    , NaviFrameView(app.getWindow())
{
    init();
}

NaviFrameController::~NaviFrameController()
{
    MSG_LOG("Destructor: ", this);
}

void NaviFrameController::init()
{
    setHwButtonListener(getEo(), this);
}

void NaviFrameController::push(FrameController &frame)
{
    NaviFrameView::push(frame);
}

void NaviFrameController::pop()
{
    if(isLastFrame())
        getApp().exit();
    else
        NaviFrameView::pop();
}

void NaviFrameController::destroy()
{
    NaviFrameView::destroy();
}

void NaviFrameController::executeCommand(AppControlCommandRef &cmd)
{
    MSG_LOG("Execute app. control command: ", cmd->getOperationMsg());

    std::string errorMsg;
    if(!getMsgEngine().isReady(errorMsg))
    {
        notification_status_message_post(errorMsg.c_str());
        return;
    }

    AppControlCommand::OperationType op = cmd->getOperationType();

    switch(op)
    {
        case AppControlCommand::OpDefault:
            execCmd(std::static_pointer_cast<AppControlCommandDefault>(cmd));
            break;

        case AppControlCommand::OpCompose:
        case AppControlCommand::OpShare:
        case AppControlCommand::OpMultiShare:
        case AppControlCommand::OpShareText:
            execComposerCmd(cmd);
            break;

        case AppControlCommand::OpUnknown:
            execCmd(cmd);
            break;

        default:
            break;
    }
}

void NaviFrameController::execCmd(AppControlCommandDefaultRef cmd)
{
    if(getItemsCount() == 0)
    {
        MsgThread *threadFrame = new MsgThread(*this);
        push(*threadFrame);
    }
}

void NaviFrameController::execComposerCmd(AppControlCommandRef cmd)
{
    if(getItemsCount() == 0)
    {
        Conversation *convFrame = new Conversation(*this, cmd);
        push(*convFrame);
    }
    else
    {
        //TODO: Handle this case (erase or save previous data)
        MSG_LOG_WARN("App was already launched! You may lost previous data!");
    }
}

void NaviFrameController::execCmd(AppControlCommandRef cmd)
{
    MSG_LOG_WARN("Unknown app control command");
}

void NaviFrameController::onHwBackButtonClicked()
{
    pop();
}

void NaviFrameController::onHwMoreButtonClicked()
{

}

