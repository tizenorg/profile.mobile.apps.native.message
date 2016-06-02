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
    class ISmilSaveAllItemViewListener;

    // SmilAttachmentItemView:
    class SmilAttachmentItemView
        : public View
    {
        public:
            SmilAttachmentItemView(Evas_Object *parent);
            virtual ~SmilAttachmentItemView();

            void setListener(ISmilAttachmentItemViewListener *l);
            void setFileName(const std::string &name);
            void setFilePath(const std::string &filePath);
            const std::string &getFilePath() const; // Return reference to file path

        private:
            ISmilAttachmentItemViewListener *m_pListener;
            std::string m_FilePath;
            Evas_Object *m_pFileButton;
            Evas_Object *m_pSaveButton;
    };

    // SmilAttachmentInfoItemView :
    class SmilAttachmentInfoItemView
        : public View
    {
        public:
            SmilAttachmentInfoItemView(Evas_Object *parent, bool manyAttachments);
            virtual ~SmilAttachmentInfoItemView();
    };

    // SmilSaveAllItemView :
    class SmilSaveAllItemView
        : public View
    {
        public:
            SmilSaveAllItemView(Evas_Object *parent, int count);
            virtual ~SmilSaveAllItemView();

            void setListener(ISmilSaveAllItemViewListener *l);

        private:
            void onLanguageChanged(Evas_Object *obj, void *eventInfo);
            void updateTitle();

        private:
            Evas_Object *m_pButton;
            ISmilSaveAllItemViewListener *m_pListener;
            const int m_Count;
    };

    class ISmilAttachmentItemViewListener
    {
        public:
            virtual ~ISmilAttachmentItemViewListener() {}
            virtual void onItemClicked(SmilAttachmentItemView &item) {};
            virtual void onSaveButtonClicked(SmilAttachmentItemView &item) {};
    };

    class ISmilSaveAllItemViewListener
    {
        public:
            virtual ~ISmilSaveAllItemViewListener() {}
            virtual void onItemClicked(SmilSaveAllItemView &item) {};
    };
}

#endif // SmilAttachmentItemView_h_
