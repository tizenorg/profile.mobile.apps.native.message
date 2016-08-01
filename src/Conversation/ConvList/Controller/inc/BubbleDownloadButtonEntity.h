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

#ifndef BubbleDownloadButtonEntity_h_
#define BubbleDownloadButtonEntity_h_

#include "BubbleDownloadButtonViewItem.h"
#include "BubbleEntity.h"

namespace Msg
{
    class BubbleDownloadButtonEntity
        : public BubbleEntity
    {
        public:
            BubbleDownloadButtonEntity(Message::Direction direction);
            virtual ~BubbleDownloadButtonEntity();

            virtual BubbleDownloadButtonViewItem *createView(Evas_Object *parent);
            virtual std::string getFilePath() const;
    };

    inline BubbleDownloadButtonEntity::BubbleDownloadButtonEntity(Message::Direction direction)
        : BubbleEntity(DownloadButtonItem, direction)
    {
    }

    inline BubbleDownloadButtonEntity::~BubbleDownloadButtonEntity()
    {
    }

    inline BubbleDownloadButtonViewItem *BubbleDownloadButtonEntity::createView(Evas_Object *parent)
    {
        auto *item = new BubbleDownloadButtonViewItem(*this, parent);
        return item;
    }

    inline std::string BubbleDownloadButtonEntity::getFilePath() const
    {
        return {};
    }
}

#endif /* BubbleDownloadButtonEntity_h_ */
