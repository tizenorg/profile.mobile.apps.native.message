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

    class BubbleEntity
    {
        friend class BubbleView;

        public:
            BubbleEntity();
            ~BubbleEntity();
            enum PartType
            {
                TextPart,   //text
                ThumbnailPart   //path to thumbnail image
            };

            /**
             * @brief Add new part to bubble entity
             * @param[in] type Set which type is @value
             * @param[in] value Resource path or raw text to display
             */
            void addPart(PartType type, const std::string &value);

        private:
            BubbleEntity(BubbleEntity&) = delete;
            BubbleEntity &operator=(BubbleEntity&) = delete;

            struct BubblePart
            {
                PartType type;
                std::string value;
            };

        private:
            std::list<BubblePart> m_Parts;
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

        private:
            void create(Evas_Object *parent);
            Evas_Object *createTextView(const std::string &text);
            Evas_Object *createThumbView(const std::string &path);
    };
}

#endif /* BubbleView_h_ */
