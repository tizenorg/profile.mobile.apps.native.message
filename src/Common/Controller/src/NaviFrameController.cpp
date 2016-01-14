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

bool NaviFrameController::execCmd(const AppControlCommand &cmd)
{
    MSG_LOG("Execute app. control command: ", cmd.getOperationMsg());

    std::string errorMsg;
    if(!getMsgEngine().isReady(errorMsg))
    {
        notification_status_message_post(errorMsg.c_str());
        return false;
    }
    return true;
}

void NaviFrameController::execCmd(const AppControlCommandMainRef &cmd)
{
    if(execCmd(*cmd))
    {
        if(isEmpty())
        {
            MsgThread *threadFrame = new MsgThread(*this);
            push(*threadFrame);
        }
    }
}

void NaviFrameController::execCmd(const AppControlComposeRef &cmd)
{
    if(execCmd(*cmd))
    {
        if(isEmpty())
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
}

void NaviFrameController::onHwBackButtonClicked()
{
    pop();
}

void NaviFrameController::onHwMoreButtonClicked()
{

}

