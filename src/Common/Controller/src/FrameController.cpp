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


#include "FrameController.h"
#include "NaviFrameController.h"
#include "Logger.h"
#include "App.h"
#include "ContactManager.h"

using namespace Msg;

FrameController::FrameController(NaviFrameController &parent)
    : ViewItemController(&parent)
    , NaviFrameItem(parent)
{
}

FrameController::~FrameController()
{
}

NaviFrameController &FrameController::getParent()
{
    return static_cast<NaviFrameController &>(ViewItemController::getParent());
}

void FrameController::onAttached(ViewItem &item)
{
    NaviFrameItem::onAttached(item);
}

void FrameController::setNaviBarTitle(const MsgAddressList &addressList)
{
    std::string title;
    if(!addressList.isEmpty())
    {
        std::string firstAddress = addressList[0].getAddress();
        ContactPersonAddressRef contactPersonAddress = getApp().getContactManager().getContactPersonAddress(firstAddress);
        if(contactPersonAddress)
            title = contactPersonAddress->getDispName();

        if(title.empty())
            title = firstAddress;

        int hidenAddresses = addressList.getLength() - 1;
        if(hidenAddresses > 0)
        {
            title += " + " + std::to_string(hidenAddresses);
            getNaviBar().showButton(NaviExpandButtonId, true);
        }
    }
    getNaviBar().showButton(NaviCenterButtonId, true);
    getNaviBar().setButtonText(NaviCenterButtonId, title);
}
