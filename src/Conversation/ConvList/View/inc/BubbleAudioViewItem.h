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

#ifndef BubbleAudioViewItem_h_
#define BubbleAudioViewItem_h_

#include "BubbleIconTextLayoutItem.h"

namespace Msg
{
    class BubbleAudioViewItem
        : public BubbleIconTextLayoutItem
    {
        public:
            BubbleAudioViewItem(BubbleEntity &entity, Evas_Object *parent);
            virtual ~BubbleAudioViewItem();
    };

    class BubbleAudioEntity
        : public BubbleEntity
    {
        public:
            BubbleAudioEntity(const std::string &filePath, const std::string &fileName, const std::string &duration);
            virtual ~BubbleAudioEntity();

            virtual BubbleAudioViewItem *createView(Evas_Object *parent);
            virtual const std::string &getFilePath() const;

        private:
            const std::string m_FilePath;
            const std::string m_FileName;
            const std::string m_Duration;
    };
}

#endif /* BubbleAudioViewItem_h_ */
