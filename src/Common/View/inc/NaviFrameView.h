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

#ifndef NaviFrameView_h_
#define NaviFrameView_h_

#include "View.h"
#include "NaviFrameItem.h"

namespace Msg
{
    class NaviFrameView
        : public View
    {
        public:
            NaviFrameView(Evas_Object *parent);
            NaviFrameView(View &parent);
            virtual ~NaviFrameView();

            /**
             * Checks is it last frame.
             */
            bool isLastFrame() const;

            /**
             * Get count of pushed frames.
             */
            int getItemsCount() const;
            /**
             * Get list of pushed frames.
             */
            NaviFrameItemList getItems() const;

            /**
             * Checks if any frame was pushed.
             */
            bool isEmpty() const;

            /**
             * Pushes new frame. Attaches @content to frame. Updates @item with created frame.
             */
            void push(NaviFrameItem &item, Evas_Object *content = nullptr);
            void push(NaviFrameItem &item, View &content);
            void pop();
            void insertToBottom(NaviFrameItem &item);
            void promote(NaviFrameItem &item);
            NaviFrameItem *getTopFrame() const;
            bool getTransitionStatus() const;

        private:
            void create(Evas_Object *parent);
            void onTransitionFinished(Evas_Object *obj, void *eventInfo);

        private:
            bool m_TransitionStatus;
    };
}

#endif /* NaviFrameView_h_ */
