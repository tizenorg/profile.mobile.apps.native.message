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

#ifndef TextPageViewItem_h_
#define TextPageViewItem_h_

#include "PageViewItem.h"
#include "LangUtils.h"

namespace Msg
{
    class ITextPageViewItemListener;

    class TextPageViewItem
        : public PageViewItem
    {
        public:
            TextPageViewItem(PageView &parent);
            virtual ~TextPageViewItem();

            virtual Type getType() const;
            virtual bool isEmpty() const;

            void setListener(ITextPageViewItemListener *l);
            void setGuideText(const TText &text);
            void clearText();
            std::string getText() const;
            std::string getPlainUtf8Text() const;

        private:
            typedef void (ITextPageViewItemListener::*ListenerMethod)(TextPageViewItem &);

            Evas_Object *createEntry(Evas_Object *parent);
            static void notifyListener(void *data, ListenerMethod method);

        private:
            Evas_Object *m_pEntry;
            ITextPageViewItemListener *m_pListener;
    };

    class ITextPageViewItemListener
    {
        public:
            virtual ~ITextPageViewItemListener() {}
            virtual void onCursorChanged(TextPageViewItem &obj) {};
            virtual void onFocused(TextPageViewItem &obj) {};
            virtual void onUnfocused(TextPageViewItem &obj) {};
            virtual void onChanged(TextPageViewItem &obj) {};
            virtual void onPreeditChanged(TextPageViewItem &obj) {};
            virtual void onPress(TextPageViewItem &obj) {};
            virtual void onClicked(TextPageViewItem &obj) {};
            virtual void onMaxLengthReached(TextPageViewItem &obj) {};
            virtual void onKeyDown(TextPageViewItem &obj) {};
            virtual void onKeyUp(TextPageViewItem &obj) {};
    };
}

#endif /* TextPageViewItem_h_ */
