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

#ifndef BubbleImageEntity_h_
#define BubbleImageEntity_h_

#include "BubbleImageViewItem.h"
#include "BubbleEntity.h"

namespace Msg
{
    class BubbleImageEntity
        : public BubbleEntity
    {
        public:
            BubbleImageEntity(const MsgConvMedia &media);
            virtual ~BubbleImageEntity();

            virtual BubbleImageViewItem *createView(Evas_Object *parent);
            virtual std::string getFilePath() const;

        private:
            const std::string m_ImgPath;
    };

    inline BubbleImageEntity::BubbleImageEntity(const MsgConvMedia &media)
        : BubbleEntity(ImageItem)
        , m_ImgPath(media.getPath())
    {
    }

    inline BubbleImageEntity::~BubbleImageEntity()
    {
    }

    inline BubbleImageViewItem *BubbleImageEntity::createView(Evas_Object *parent)
    {
        auto *item = new BubbleImageViewItem(*this, parent, m_ImgPath);
        return item;
    }

    inline std::string BubbleImageEntity::getFilePath() const
    {
        return m_ImgPath;
    }

}

#endif /* BubbleImageEntity_h_ */
