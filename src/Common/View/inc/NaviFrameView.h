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
    /**
     * @brief Wraps all basic Naviframe operations.
     */
    class NaviFrameView
        : public View
    {
        public:
            NaviFrameView(Evas_Object *parent);
            NaviFrameView(View &parent);
            virtual ~NaviFrameView();

            /**
             * @brief Checks is it last frame.
             */
            bool isLastFrame() const;

            /**
             * @brief Get count of pushed frames.
             */
            int getItemsCount() const;
            /**
             * @brief Get list of pushed frames.
             */
            NaviFrameItemList getItems() const;

            /**
             * @brief Checks if any frame was pushed.
             */
            bool isEmpty() const;

            /**
             * @brief Pushes new frame. Attaches @content to frame. Updates @item with created frame.
             */
            void push(NaviFrameItem &item, Evas_Object *content = nullptr);
            void push(NaviFrameItem &item, View &content);

            /**
             * @brief Pops item from Naviframe.
             */
            void pop();

            /**
             * @brief Inserts specified item to bottom of item's stack.
             * @param[in] item an item to be inserted.
             */
            void insertToBottom(NaviFrameItem &item);

            /**
             * @brief Promote an item already in the naviframe stack to the top of the stack
             * @param[in] item an item to be promoted.
             */
            void promote(NaviFrameItem &item);

            /**
             * @brief Gets top frame from items-stack.
             * @return Top naviframe-item or nullptr if item-stack is empty.
             */
            NaviFrameItem *getTopFrame() const;

            /**
             * Checks if item transition is in progress.
             * @return true if transition is in progress, false otherwise.
             */
            bool getTransitionStatus() const;

        private:
            void create(Evas_Object *parent);
            void onTransitionFinished(Evas_Object *obj, void *eventInfo);

        private:
            bool m_TransitionStatus;
    };
}

#endif /* NaviFrameView_h_ */
