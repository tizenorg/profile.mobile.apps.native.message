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

#ifndef BubbleEntity_h_
#define BubbleEntity_h_

#include "View.h"
#include "BubbleViewItem.h"
#include "MsgConvMedia.h"
#include "FileUtils.h"
#include "Message.h"

#include <string>

namespace Msg
{
    class BubbleEntity
    {
        public:
            enum Type
            {
                TextItem,
                ImageItem,
                AudioItem,
                VideoItem,
                ContactItem,
                CalendarEventItem,
                UnknownFileItem,
                DownloadButtonItem
            };

        public:
            BubbleEntity(Type type, Message::Direction direction);
            virtual ~BubbleEntity();

            Type getType() const;
            Message::Direction getDirection() const;
            virtual std::string getFilePath() const = 0;
            virtual BubbleViewItem *createView(Evas_Object *parent) = 0;

            static std::string getFileName(const MsgConvMedia &media);

        private:
            Type m_Type;
            Message::Direction m_Direction;
    };

    inline BubbleEntity::BubbleEntity(Type type, Message::Direction direction)
        : m_Type(type)
        , m_Direction(direction)
    {
    }

    inline BubbleEntity::~BubbleEntity()
    {
    }

    inline BubbleEntity::Type BubbleEntity::getType() const
    {
        return m_Type;
    }

    inline Message::Direction BubbleEntity::getDirection() const
    {
        return m_Direction;
    }

    inline std::string BubbleEntity::getFileName(const MsgConvMedia &media)
    {
        std::string res = media.getName();
        if(res.empty())
            res = FileUtils::getFileName(media.getPath());
        return res;
    }
}

#endif /* BubbleEntity_h_ */
