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

#ifndef ImagePageViewItem_h_
#define ImagePageViewItem_h_

#include "MediaPageViewItem.h"

#include <vector>

namespace Msg
{
    class ImagePageViewItem
        : public MediaPageViewItem
    {
        public:
            ImagePageViewItem(PageView &parent, const std::string &reourcePath, long long fileSize, const std::string &imagePath);
            virtual ~ImagePageViewItem();

            void showPlayIcon();
            const std::string &getImagePath() const;
            virtual Type getType() const;
            virtual bool isEmpty() const;
            virtual void highlight(bool value);
            virtual std::string getFileName() const;

        private:
            Evas_Object *createImageLayout(Evas_Object *parent);
            Evas_Object *createRect(Evas_Object *parent);
            Evas_Object *createImage(Evas_Object *parent);

        private:
            Evas_Object *m_pImageLayout;
            const std::string m_ImagePath;
    };
}

#endif /* ImagePageViewItem_h_ */
