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

        MsgThread *thread = getFrame<MsgThread>(); // Check if thread is open
        if(!thread)
            insertToBottom(*new MsgThread(*this)); // Push thread list to the bottom

        Conversation *conv = getFrame<Conversation>(); // Check if conversation is open
        if(type != AppControlDefault::MainType)
        {
            if(conv)
            {
                promote(*conv);
                conv->execCmd(cmd);
            }
            else if(!conv)
            {
                if(type == AppControlDefault::ViewType || type == AppControlDefault::ReplyType ||
                        (type == AppControlDefault::NotificationType && getMsgEngine().getStorage().getUnreadThreadCount() == 1))
                {
                        Conversation *conversation = new Conversation(*this);
                        conversation->execCmd(cmd);
                        push(*conversation);
                }
            }
        }
    }
}

void NaviFrameController::execCmd(const AppControlComposeRef &cmd)
{
    if(execCmd(*cmd))
    {
        Conversation *conv = getFrame<Conversation>();
        if(conv)
        {
            conv->execCmd(cmd);
        }
        else
        {
            Conversation *conv = new Conversation(*this);
            conv->execCmd(cmd);
            push(*conv);
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

