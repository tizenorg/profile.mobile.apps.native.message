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

#ifndef SmilPage_h_
#define SmilPage_h_

#include "SmilPageLayout.h"
#include "MsgPage.h"
#include "MsgAttachment.h"
#include "SmilAttachmentItemView.h"
#include "FileViewer.h"

#include <list>

namespace Msg
{
    class SmilImageItemView;

    class SmilPage
        : public SmilPageLayout
        , private ISmilSaveAllItemViewListener
        , private ISmilAttachmentItemViewListener
    {
        public:
            SmilPage(Evas_Object *parent, const MsgPage &page);
            SmilPage(Evas_Object *parent, const MsgAttachmentList &list);
            virtual ~SmilPage();

            int getDuration() const;
            bool hasMedia() const;
            bool hasInvalidMedia() const;
            bool hasVideo() const;
            bool hasAudio() const;
            bool hasAnimation() const;
            void playAnimation(bool play);
            Evas_Object *getVideoSink() const;
            std::string getMediaPath() const;
            const std::list<std::string> &getAttachments() const;

        private:
            const MsgMedia *getMedia(const MsgPage &page, MsgMedia::Type type) const;
            void build(const MsgPage &page);
            void build(const MsgAttachmentList &list);
            void buildImage(const MsgMedia& media);
            void buildText(const MsgMedia& media);
            void buildAudio(const MsgMedia& media);
            void buildVideo(const MsgMedia& media);
            void buildSaveAllItem(int attachmentCount);
            void buildAttachmentInfo(int attachmentCount);
            void buildAttachment(const MsgAttachment& attachment);

            // ISmilAttachmentItemViewListener:
            virtual void onItemClicked(SmilAttachmentItemView &item);
            virtual void onSaveButtonClicked(SmilAttachmentItemView &item);

            // ISmilSaveAllItemViewListener:
            virtual void onItemClicked(SmilSaveAllItemView &item);

        private:
            int m_Duration;
            std::string m_MediaPath;
            Evas_Object *m_pVideoSink;
            bool m_HasAudio;
            SmilImageItemView *m_pImageItem;
            std::list<std::string> m_Attachments;
            FileViewer m_FileViewer;
            bool m_HasInvalidMedia;
    };
}

#endif /* SmilPage_h_ */
