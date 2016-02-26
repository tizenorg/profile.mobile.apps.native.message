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

#ifndef Body_h_
#define Body_h_

#include "BodyView.h"
#include "MessageSMS.h"
#include "MessageMms.h"
#include "WorkingDir.h"
#include "AppControlCompose.h"

#include <list>
#include <Ecore.h>

namespace Msg
{
    class IBodyListener;
    class MsgEngine;

    struct BodySmsSize
    {
        int charsLeft;
        int smsCount;
    };

    class Body
        : public BodyView
    {
        public:
            Body(Evas_Object *parent, MsgEngine &msgEngine);
            virtual ~Body();

            void setListener(IBodyListener *listener);

            bool addMedia(const std::list<std::string> &fileList);
            bool addMedia(const std::string &filePath);

            bool isMms() const;
            BodySmsSize getSmsSize() const;
            long long getMmsSize() const;
            void read(Message &msg);
            void write(const Message &msg);
            void execCmd(const AppControlComposeRef &cmd);

        private:
            void read(MessageSMS &msg);
            void read(MessageMms &msg);
            void readText(MsgPage &msgPage, const PageView &pageView);
            void readSound(MsgPage &msgPage, const PageView &pageView);
            void readImage(MsgPage &msgPage, const PageView &pageView);
            void readVideo(MsgPage &msgPage, const PageView &pageView);
            void readAttachments(MessageMms &msg);
            void write(const MessageSMS &msg);
            void write(const MessageMms &msg);
            void writePage(const MsgPage &msgPage, PageView &pageView);
            void writeText(const MsgMedia &msgMedia, PageView &pageView);
            void writeImage(const MsgMedia &msgMedia, PageView &pageView);
            void writeVideo(const MsgMedia &msgMedia, PageView &pageView);
            void writeSound(const MsgMedia &msgMedia, PageView &pageView);
            void writeAttachments(const MessageMms &msg);
            bool isMms(const PageView &page) const;
            void writeTextToFile(TextPageViewItem &item);
            bool addVideo(PageView &page, const std::string &videoFilePath);

            // BodyView:
            virtual void onContentChanged();
            virtual void onItemDelete(PageViewItem &item);
            virtual void onItemDelete(BodyAttachmentView &item);

        private:
            IBodyListener *m_pListener;
            MsgEngine &m_MsgEngine;
            WorkingDir m_WorkingDir;
            Ecore_Idler *m_pOnChangedIdler;
    };

    class IBodyListener
    {
        public:
            virtual ~IBodyListener() {}

            virtual void onChanged(Body &body) {};
    };
}

#endif /* Body_h_ */
