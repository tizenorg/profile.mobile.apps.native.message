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

#ifndef Page_h_
#define Page_h_

#include "PageView.h"
#include "MsgEngine.h"
#include "WorkingDir.h"
#include "TextPageViewItem.h"

namespace Msg
{
    class Body;
    class Page
        : public PageView
    {
        public:


        public:
            Page(Body &parent, WorkingDirRef workingDir);
            virtual ~Page();

            int getAttachmentsCount() const;
            const MsgTextMetric &getTextMetric();
            long long getSize();
            bool isMms();
            void read(MsgPage &msgPage);
            void write(const MsgPage &msgPage);
            bool addMedia(const std::string &filePath);

        private:
            void updateMsgMetricIfNeeded();

            void readText(MsgPage &msgPage);
            void readSound(MsgPage &msgPage);
            void readImage(MsgPage &msgPage);
            void readVideo(MsgPage &msgPage);

            void writeText(const MsgMedia &msgMedia);
            void writeImage(const MsgMedia &msgMedia);
            void writeVideo(const MsgMedia &msgMedia);
            void writeSound(const MsgMedia &msgMedia);
            void writeTextToFile(TextPageViewItem &item);

            void addVideo(const std::string &videoPath);
            void addImage(const std::string &filePath);
            void addSound(const std::string &filePath, const std::string &fileName = "");

        private:
            Body &m_Body;
            MsgTextMetric m_MsgMetric;
            WorkingDirRef m_WorkingDir;
    };
}

#endif /* Page_h_ */
