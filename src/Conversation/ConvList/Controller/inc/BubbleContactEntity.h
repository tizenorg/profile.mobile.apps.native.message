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

#ifndef BubbleContactEntity_h_
#define BubbleContactEntity_h_

#include "BubbleContactViewItem.h"
#include "BubbleEntity.h"
#include "App.h"
#include "Contact.h"

namespace Msg
{
    class BubbleContactEntity
        : public BubbleEntity
    {
        public:
            BubbleContactEntity(App &app, const MsgConvMedia &media);
            virtual ~BubbleContactEntity();

            virtual BubbleContactViewItem *createView(Evas_Object *parent);
            virtual std::string getFilePath() const;

        private:
            ThumbnailMaker &m_ThumbMaker;
            std::string m_FilePath;
            std::string m_Name;
            std::string m_Address;
            std::string m_ThumbPath;
            ContactRef m_Contact; // Thumbnail file holder
    };
}

#endif /* BubbleContactEntity_h_ */
