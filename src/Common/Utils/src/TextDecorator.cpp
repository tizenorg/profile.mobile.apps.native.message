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

#include "TextDecorator.h"
#include <sstream>
#include <assert.h>
#include <string.h>

using namespace Msg;

const std::string TextStyle::defaultColor = "#000000FF";
const int TextStyle::defaultFontSize = 28;

#define OPEN_TAG(name, val) "<" name "=" << (val) << ">"
#define CLOSE_TAG(name) "</" name ">"

namespace
{
    const char *alignAsString(TextAlign align)
    {
        switch(align)
        {
            case TextAlign::Center:
                return "center";
            case TextAlign::Left:
                return "left";
            case TextAlign::Right:
                return "right";
            case TextAlign::None:
            default:
                break;
        }

        assert(false);
        return "";
    }
}

TextStyle::TextStyle()
    : m_Color(defaultColor)
    , m_Align(TextAlign::None)
    , m_Size(defaultFontSize)
{
}

TextStyle::TextStyle(int size, const std::string &color, TextAlign align)
    : m_Color(color)
    , m_Align(align)
    , m_Size(size)
{

}

TextStyle::~TextStyle()
{
}

void TextStyle::setColor(const std::string &color)
{
    m_Color = color;
}

void TextStyle::setSize(int size)
{
    m_Size = size;
}

void TextStyle::setAlign(TextAlign align)
{
    m_Align = align;
}

const std::string &TextStyle::getColor() const
{
    return m_Color;
}

int TextStyle::getSize() const
{
    return m_Size;
}

TextAlign TextStyle::getAlign() const
{
    return m_Align;
}

std::string TextDecorator::make(const std::string &text, const TextStyle &style)
{
    return make(text, style.getSize(), style.getColor(), style.getAlign());
}

std::string TextDecorator::make(const std::string &text,
                                int size,
                                const std::string &color,
                                TextAlign align)
{
    std::ostringstream ss;
    ss << OPEN_TAG("font_size", size) << OPEN_TAG("color", color);
    if (align != TextAlign::None)
    {
        ss << OPEN_TAG("align", alignAsString(align));
    }
    ss << text;
    if (align != TextAlign::None)
    {
        ss << CLOSE_TAG("align");
    }
    ss << CLOSE_TAG("color") << CLOSE_TAG("font_size");
    return ss.str();
}

std::string TextDecorator::highlightKeyword(const std::string &str, const std::string &searchWord)
{
    if(str.empty() || searchWord.empty())
        return str;

    char *found = strcasestr((char*)str.c_str(), (char*)searchWord.c_str());
    if(!found)
        return str;

    std::string res;
    res.reserve(str.length() + 32);

    int diff = found - str.c_str();
    std::string firstPart = std::string(str.begin(), str.begin() + diff);
    std::string lastPart = std::string(str.begin() + diff + searchWord.length(), str.end());

    res += firstPart;
    res += "<match>";
    res += searchWord;
    res += "</match>";
    res += lastPart;

    return res;
}
