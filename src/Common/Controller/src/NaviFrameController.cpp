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

void NaviFrameController::insertBefore(FrameController &insert, FrameController &before)
{
    NaviFrameView::insertBefore(insert, before);
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
        if(isEmpty())
        {
            // If we open app with app control operation in a first time
            if(type != AppControlDefault::ReplyType) // If we came not after reply button was clicked, we need to open message thread list menu
                push(*new MsgThread(*this));

            // In case when we clicked Reply or View buttons, we should move to needed conversation
            if(type == AppControlDefault::ReplyType || type == AppControlDefault::ViewType)
            {
                Conversation *conv = new Conversation(*this);
                conv->execCmd(cmd);
                push(*conv);
            }
            else if(type == AppControlDefault::NotificationType)
            {
                // When we get from notification panel and unread thread is only one, we should move to it.
                // In another way, to stay on thread list menu
                if(getMsgEngine().getStorage().getUnreadThreadCount() == 1)
                {
                    Conversation *conv = new Conversation(*this);
                    conv->execCmd(cmd);
                    push(*conv);
                }
            }
        }
        else
        {
            // If we open app with app control operation when it is working
            MsgThread *thread = getFrame<MsgThread>(); // Check if thread list is open
            Conversation *conv = getFrame<Conversation>(); // Check if conversation is open
            if(type != AppControlDefault::MainType)
            {
                if(conv && !thread) // Case if we have opened conversation but thread list is closed
                {
                    // If app was opened from notification panel
                    if(type == AppControlDefault::NotificationType)
                    {
                        // If unread threads are more than one and thread list was not opened, but conversation
                        // is opened we should to insert thread list frame before opened conversation, for case
                        // when you press the back button and then we went back to the thread list but did not close the application
                        if(getMsgEngine().getStorage().getUnreadThreadCount() > 1)
                            insertBefore(*new MsgThread(*this), *conv);
                    }
                    else if(type == AppControlDefault::ViewType)
                    {
                        // The same as in the case with unread threads.
                        insertBefore(*new MsgThread(*this), *conv);
                    }
                    conv->execCmd(cmd);
                }
                else if(!conv && thread) // Case if just thread list is opened
                {
                    if(type == AppControlDefault::NotificationType)
                    {
                        if(getMsgEngine().getStorage().getUnreadThreadCount() == 1)
                        {
                            Conversation *conversation = new Conversation(*this);
                            conversation->execCmd(cmd);
                            push(*conversation);
                        }
                    }
                    else if(type == AppControlDefault::ViewType || type == AppControlDefault::ReplyType)
                    {
                        Conversation *conversation = new Conversation(*this);
                        conversation->execCmd(cmd);
                        push(*conversation);
                    }
                }
                else if(conv && thread) // Case if canversation and thread list are opened
                {
                    conv->execCmd(cmd);
                }
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
            Conversation *conv = getFrame<Conversation>();
            if(conv)
                conv->execCmd(cmd);
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

