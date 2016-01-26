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

void NaviFrameController::execCmd(const AppControlDefaultRef &cmd)
{
    if(execCmd(*cmd))
    {
        AppControlDefault::DefaultType type = cmd->getDefaultType();

        Conversation *conv = getFrame<Conversation>();
        if(conv)
            pop();

        MsgThread *thread = getFrame<MsgThread>();
        if(!thread && type != AppControlDefault::ReplyType)
            push(*new MsgThread(*this));

        if(type == AppControlDefault::ReplyType || type == AppControlDefault::ViewType)
        {
            Conversation *conv = new Conversation(*this);
            conv->execCmd(cmd);
            push(*conv);
        }
        else if(type == AppControlDefault::NotificationType)
        {
            if(getMsgEngine().getStorage().getUnreadThreadCount() == 1)
            {
                Conversation *conv = new Conversation(*this);
                conv->execCmd(cmd);
                push(*conv);
            }
        }
    }
}

void NaviFrameController::execCmd(const AppControlComposeRef &cmd)
{
    if(execCmd(*cmd))
    {
        if(isEmpty())
        {
            Conversation *conv = new Conversation(*this);
            conv->execCmd(cmd);
            push(*conv);
        }
        else
        {
            //TODO: Handle this case (erase or save previous data)
            MSG_LOG_WARN("App was already launched! You may lost previous data!");
        }
    }
}

template<typename T>
T *NaviFrameController::getFrame() const
{
    auto items = getItems();
    for(NaviFrameItem *item : items)
    {
        T *frame = dynamic_cast<T*>(item);
        if(frame)
            return frame;
    }
    return nullptr;
}

void NaviFrameController::onHwBackButtonClicked()
{
    pop();
}

void NaviFrameController::onHwMoreButtonClicked()
{

}

