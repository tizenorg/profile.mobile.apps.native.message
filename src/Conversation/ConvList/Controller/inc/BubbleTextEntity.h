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

#ifndef BubbleTextEntity_h_
#define BubbleTextEntity_h_

#include "BubbleTextViewItem.h"
#include "BubbleBgEntity.h"

namespace Msg
{
    class BubbleTextEntity
        : public BubbleBgEntity
    {
        public:
            BubbleTextEntity(BubbleBgViewItem::BgType type, Message::Direction direction, const std::string &text);
            virtual ~BubbleTextEntity();

            virtual std::string getFilePath() const;
            virtual BubbleTextViewItem *createView(Evas_Object *parent);

        private:
            std::string m_Text;
    };

    inline BubbleTextEntity::BubbleTextEntity(BubbleBgViewItem::BgType type, Message::Direction direction, const std::string &text)
        : BubbleBgEntity(TextItem, type, direction)
        , m_Text(text)
    {
    }

    inline BubbleTextEntity::~BubbleTextEntity()
    {
    }

    inline std::string BubbleTextEntity::getFilePath() const
    {
        static std::string empty;
        return empty;
    }

    inline BubbleTextViewItem *BubbleTextEntity::createView(Evas_Object *parent)
    {
        auto *item = new BubbleTextViewItem(*this ,parent, m_BgType, m_Text);
        return item;
    }
}

#endif /* BubbleTextEntity_h_ */
