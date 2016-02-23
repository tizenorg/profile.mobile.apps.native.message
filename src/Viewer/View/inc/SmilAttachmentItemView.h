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

#ifndef SmilAttachmentItemView_h_
#define SmilAttachmentItemView_h_

#include "View.h"

namespace Msg
{
    class ISmilAttachmentItemViewListener;

    class SmilAttachmentItemView
        : public View
    {
        public:
            SmilAttachmentItemView(Evas_Object *parent, const std::string &thumbFilePath = "");
            virtual ~SmilAttachmentItemView();

            void setListener(ISmilAttachmentItemViewListener *l);
            void setFileName(const std::string &name);
            void setFileSize(const std::string &size);
            void setFilePath(const std::string &filePath);
            const std::string &getFilePath() const;

        private:
            void onClickedCb(Evas_Object *obj, const char *emission, const char *source);
            Evas_Object *createItem(Evas_Object *parent, const std::string &thumbFilePath);
            Evas_Object *createIcon(Evas_Object *parent, const std::string &thumbFilePath);

        private:
            Evas_Object *m_pItem;
            ISmilAttachmentItemViewListener *m_pListener;
            std::string m_FilePath;
    };

    class SmilAttachmentInfoItemView
        : public View
    {
        public:
            SmilAttachmentInfoItemView(Evas_Object *parent, bool manyAttachments);
            virtual ~SmilAttachmentInfoItemView();
    };

    class ISmilAttachmentItemViewListener
    {
        public:
            virtual ~ISmilAttachmentItemViewListener() {}
            virtual void onItemClicked(SmilAttachmentItemView &item) {};
    };
}

#endif // SmilAttachmentItemView_h_
