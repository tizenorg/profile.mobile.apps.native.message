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

#ifndef BodyAttachmentViewItem_h_
#define BodyAttachmentViewItem_h_

#include "BodyViewItem.h"

namespace Msg
{
    class BodyView;
    class IBodyAttachmentViewListener;

    class BodyAttachmentViewItem
        : public BodyViewItem
    {
        public:
            BodyAttachmentViewItem(BodyView &parent, const std::string &resourePath, long long fileSize, const std::string &dispName = "");
            virtual ~BodyAttachmentViewItem();

            void setListener(IBodyAttachmentViewListener *listener);
            const std::string &getResourcePath() const;
            const std::string &getFileName() const;
            long long getFileSize() const;

        private:
            Evas_Object *createLayout(Evas_Object *parent);
            Evas_Object *createButton(Evas_Object *parent);
            Evas_Object *createLabel(Evas_Object *parent, const std::string &name);

        private:
            Evas_Object *m_pLayaout;
            IBodyAttachmentViewListener *m_pListener;
            const std::string m_ResourePath;
            std::string m_FileName;
            long long m_FileSize;
    };

    class IBodyAttachmentViewListener
    {
        public:
            virtual ~IBodyAttachmentViewListener() {}
            virtual void onClicked(BodyAttachmentViewItem &item) {};
            virtual void onDelete(BodyAttachmentViewItem &item) {};
    };
}

#endif /* BodyAttachmentViewItem_h_ */
