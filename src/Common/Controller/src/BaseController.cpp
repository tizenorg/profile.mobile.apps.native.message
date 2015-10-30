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


#include "BaseController.h"
#include "App.h"
#include "Logger.h"

using namespace Msg;

BaseController::BaseController(App &app)
    : m_App(app)
    , m_pParent(nullptr)

{
}

BaseController::BaseController(BaseController *parent)
    : m_App(parent->getApp())
    , m_pParent(parent)
{
}

BaseController::~BaseController()
{
}

BaseController &BaseController::getParent()
{
    MSG_ASSERT(m_pParent, "Parent is null pointer");
    return *m_pParent;
}

void BaseController::onPause()
{

}

void BaseController::onResume()
{

}

MsgEngine &BaseController::getMsgEngine()
{
    return m_App.getMsgEngine();
}

const MsgEngine &BaseController::getMsgEngine() const
{
    return m_App.getMsgEngine();
}


