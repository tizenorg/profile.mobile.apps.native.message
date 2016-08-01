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

#ifndef BubbleAudioEntity_h_
#define BubbleAudioEntity_h_

#include "BubbleAudioViewItem.h"
#include "BubbleBgEntity.h"

namespace Msg
{
    class BubbleAudioEntity
        : public BubbleBgEntity
    {
        public:
            BubbleAudioEntity(const MsgConvMedia &media, BubbleBgViewItem::BgType bgType, Message::Direction direction);
            virtual ~BubbleAudioEntity();

            virtual BubbleAudioViewItem *createView(Evas_Object *parent);
            virtual std::string getFilePath() const;

        private:
            const std::string m_FilePath;
            const std::string m_FileName;
            const std::string m_Duration;
    };
}

#endif /* BubbleAudioEntity_h_ */
