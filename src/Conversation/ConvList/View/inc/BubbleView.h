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

#ifndef BubbleView_h_
#define BubbleView_h_

#include "View.h"
#include "MsgTypes.h"

#include <string>
#include <list>

namespace Msg
{
    class BubbleView;
    class IBubbleViewListener;

    class BubbleEntity
    {
        friend class BubbleView;

        public:
            BubbleEntity();
            ~BubbleEntity();
            enum ItemType
            {
                TextItem,
                ImageItem,
                VideoItem,
                DownloadButtonItem
            };

            /**
             * @brief Add new item to bubble entity
             * @param[in] type Set which type is @value
             * @param[in] value Resource path or raw text to display
             */
            void addItem(ItemType type, const std::string &value = std::string());

        private:
            BubbleEntity(BubbleEntity&) = delete;
            BubbleEntity &operator=(BubbleEntity&) = delete;

            struct Item
            {
                ItemType type;
                std::string value;
            };

        private:
            std::vector<Item> m_Items;
    };

    class BubbleView
        : public View
    {
        public:
            BubbleView(Evas_Object *parent);
            virtual ~BubbleView();

            /**
             * @brief Draw content from @entity
             * @param[in] entity Filled list of contents
             */
            void fill(const BubbleEntity &entity);
            void setListener(IBubbleViewListener *listener);

        private:
            void create(Evas_Object *parent);
            Evas_Object *createText(const std::string &text);
            Evas_Object *createImage(const std::string &path);
            Evas_Object *createVideo(const std::string &path);
            Evas_Object *createDownloadButton();
            void onDownloadPressed(Evas_Object *obj, void *event_info);

        private:
            IBubbleViewListener *m_pListener;
    };

    class IBubbleViewListener
    {
        public:
            virtual ~IBubbleViewListener() {}
            virtual void onDownloadButtonClicked() {};
    };
}

#endif /* BubbleView_h_ */
