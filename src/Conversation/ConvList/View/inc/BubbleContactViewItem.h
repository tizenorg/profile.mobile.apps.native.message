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

#ifndef BubbleContactViewItem_h_
#define BubbleContactViewItem_h_

#include "BubbleIconTextLayoutItem.h"
#include "ThumbnailMaker.h"

namespace Msg
{
    class BubbleContactViewItem
        : public BubbleIconTextLayoutItem
    {
        public:
            BubbleContactViewItem(BubbleEntity &entity, Evas_Object *parent, LayoutType type);
            virtual ~BubbleContactViewItem();
    };

    class BubbleContactEntity
        : public BubbleEntity
    {
        public:
            BubbleContactEntity(ThumbnailMaker &thumbMaker, const std::string &filePath, const std::string &fileName);
            BubbleContactEntity(ThumbnailMaker &thumbMaker, const std::string &filePath, const std::string &dispName, const std::string &address);
            virtual ~BubbleContactEntity();

            void setThumbId(ThumbId id);
            virtual BubbleContactViewItem *createView(Evas_Object *parent);
            virtual const std::string &getFilePath() const;

        private:
            ThumbnailMaker &m_ThumbMaker;
            const std::string m_FilePath;
            const std::string m_Name;
            const std::string m_Address;
            ThumbId m_ThumbId;
    };
}

#endif /* BubbleContactViewItem_h_ */
