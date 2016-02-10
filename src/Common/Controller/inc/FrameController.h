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

#ifndef FrameController_h_
#define FrameController_h_

#include "ViewItemController.h"
#include "NaviFrameItem.h"
#include "NaviFrameController.h"
#include "MsgAddress.h"

namespace Msg
{
    class NaviFrameController;

    class FrameController
        : public ViewItemController
        , public NaviFrameItem
    {
        public:
            FrameController(NaviFrameController &parent);
            virtual ~FrameController();

            virtual NaviFrameController &getParent();

        protected:
            void setNaviBarTitle(const MsgAddressList &addressList);

            // NaviFrameItem
            virtual void onAttached(ViewItem &item);
    };
}

#endif /* FrameController */
