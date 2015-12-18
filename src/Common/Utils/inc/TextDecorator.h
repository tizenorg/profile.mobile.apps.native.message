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

#ifndef TextDecorator_h_
#define TextDecorator_h_

#include <string>

namespace Msg
{
    enum class TextAlign
    {
        None,
        Left,
        Center,
        Right
    };

    class TextStyle
    {
        public:
            static const std::string defaultColor;
            static const int defaultFontSize;

        public:
            TextStyle();
            TextStyle(int size, const std::string &color, TextAlign align = TextAlign::None);
            ~TextStyle();

            void setColor(const std::string &color);
            void setSize(int size);
            void setAlign(TextAlign align);

            const std::string &getColor() const;
            int getSize() const;
            TextAlign getAlign() const;

        private:
            std::string m_Color;
            TextAlign m_Align;
            int m_Size;
    };

    class TextDecorator
    {
        public:
            static std::string make(const std::string &text, const TextStyle &style);
            static std::string make(const std::string &text,
                                    int size = TextStyle::defaultFontSize,
                                    const std::string &color = TextStyle::defaultColor,
                                    TextAlign align = TextAlign::None);

            static std::string highlightKeyword(const std::string &str, const std::string &searchWord);
    };
}

#endif /* TextDecorator_h_ */
