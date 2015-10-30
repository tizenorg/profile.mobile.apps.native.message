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

#ifndef PageView_h_
#define PageView_h_

#include "View.h"

namespace Msg
{
    class BodyView;

    class BodyViewItem
        : public View
    {
        public:
            enum Type
            {
                SeparatorType,
                PageType
            };

        public:
            BodyViewItem(Evas_Object *parent, Type type);
            virtual ~BodyViewItem();

            Type getType() const;

        protected:
            std::string getEdjPath() const;
            void setChild(Evas_Object *item);

        private:
            Type m_Type;
    };

    class PageSeparator
        : public BodyViewItem
    {
        public:
            PageSeparator(BodyView &parent);
            virtual ~PageSeparator();

            void setText(const std::string &text);

        private:
            Evas_Object *m_pLayout;
    };

    class PageView
        : public BodyViewItem
    {
        public:
            PageView(BodyView &parent);
            virtual ~PageView();

            void setGuideText(const TText &text);
            void clearText();
            std::string getText() const;
            std::string getPlainUtf8Text() const;
            BodyView &getBody();
            const BodyView &getBody() const;
            bool isEmpty() const;

        private:
            Evas_Object *createLayout(Evas_Object *parent);
            Evas_Object *createBox(Evas_Object *parent);
            Evas_Object *createEntry(Evas_Object *parent);

        private:
            Evas_Object *m_pEntry;
            BodyView &m_Body;
    };
}

#endif /* PageView_h_ */
