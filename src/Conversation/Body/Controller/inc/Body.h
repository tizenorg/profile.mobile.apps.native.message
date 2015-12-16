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
#include "MediaPageViewItem.h"
#include "TextPageViewItem.h"

namespace Msg
{
    class IBodyListener;
    class MsgEngine;
    class TextPageViewItem;
    class ImagePageViewItem;
    class AudioPageViewItem;

    struct BodySmsSize
    {
        int charsLeft;
        int smsCount;
    };

    class Body
        : public BodyView
        , private ITextPageViewItemListener
        , private IMediaPageViewItemListener
    {
        public:
            Body(Evas_Object *parent, MsgEngine &msgEngine);
            virtual ~Body();

            void setFocus(bool focus);
            void setListener(IBodyListener *listener);
            void clear();
            bool isEmpty() const;
            bool addMedia(const std::string &filePath);

            bool isMms() const;
            BodySmsSize getSmsSize() const;
            long long getMmsSize() const;
            void read(Message &msg);
            void write(const Message &msg);

        private:
            void read(MessageSMS &msg);
            void read(MessageMms &msg);
            void write(const MessageSMS &msg);
            void write(const MessageMms &msg);

            PageView *addPage();
            bool isMms(const PageView &page) const;

            // ITextPageViewItemListener:
            virtual void onChanged(TextPageViewItem &item);
            virtual void onCursorChanged(TextPageViewItem &item) {};
            virtual void onFocused(TextPageViewItem &item) {};
            virtual void onUnfocused(TextPageViewItem &item) {};
            virtual void onPreeditChanged(TextPageViewItem &item) {};
            virtual void onPress(TextPageViewItem &item) {};
            virtual void onClicked(TextPageViewItem &item) {};
            virtual void onMaxLengthReached(TextPageViewItem &item) {};
            virtual void onKeyDown(TextPageViewItem &item) {};
            virtual void onKeyUp(TextPageViewItem &item) {};

            // IMediaPageViewItemListener:
            virtual void onClicked(MediaPageViewItem &item) {};
            virtual void onPressed(MediaPageViewItem &item) {};
            virtual void onUnpressed(MediaPageViewItem &item) {};
            virtual void onFocused(MediaPageViewItem &item) {};
            virtual void onUnfocused(MediaPageViewItem &item) {};

        private:
            IBodyListener *m_pListener;
            PageView *m_pDefaultPage;
            MsgEngine &m_MsgEngine;
            WorkingDir m_WorkingDir;
    };

    class IBodyListener
    {
        public:
            virtual ~IBodyListener() {}

            virtual void onChanged(Body &body) {};
            virtual void onFoucused(Body &body) {};
            virtual void onUnfoucused(Body &body) {};
    };
}

#endif /* Body_h_ */
