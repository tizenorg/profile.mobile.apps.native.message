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
#include "Page.h"

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

            const Page &getDefaultPage() const;
            Page &getDefaultPage();
            std::string getText() const;
            void setFocus(bool focus);
            void setListener(IBodyListener *listener);
            BodySmsSize getSmsSize() const;
            long long getMmsSize() const;
            void clear();
            bool isEmpty() const;

        private:
            // Page event:
            virtual void onCursorChanged(PageView &obj);
            virtual void onFocused(PageView &obj);
            virtual void onUnfocused(PageView &obj);
            virtual void onChanged(PageView &obj);
            virtual void onPreeditChanged(PageView &obj);
            virtual void onPress(PageView &obj);
            virtual void onClicked(PageView &obj);
            virtual void onMaxLengthReached(PageView &obj);
            virtual void onKeyDown(PageView &obj);
            virtual void onKeyUp(PageView &obj);

        private:
            IBodyListener *m_pListener;
            Page *m_pDefaultPage;
            MsgEngine &m_MsgEngine;
    };

    class IBodyListener
    {
        public:
            virtual ~IBodyListener() {}

            virtual void onChanged(Page &page) {};
            virtual void onFoucused(Page &page) {};
            virtual void onUnfoucused(Page &page) {};
    };
}

#endif /* Body_h_ */
