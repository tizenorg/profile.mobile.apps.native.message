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

#ifndef BubbleVideoEntity_h_
#define BubbleVideoEntity_h_

#include "BubbleVideoViewItem.h"
#include "BubbleEntity.h"
#include "WorkingDir.h"

namespace Msg
{
    class BubbleVideoEntity
        : public BubbleEntity
    {
        public:
            BubbleVideoEntity(WorkingDir &workingDir, const MsgConvMedia &media);
            virtual ~BubbleVideoEntity();

            virtual BubbleVideoViewItem *createView(Evas_Object *parent);
            virtual std::string getFilePath() const;

        private:
            std::string m_VideoPath;
            std::string m_ImgPath;
    };
}

#endif /* BubbleVideoEntity_h_ */
