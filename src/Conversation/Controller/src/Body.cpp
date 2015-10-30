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

#include "Body.h"
#include "Page.h"
#include "MsgEngine.h"

using namespace Msg;

Body::Body(Evas_Object *parent, MsgEngine &msgEngine)
    : BodyView(parent)
    , m_pListener(nullptr)
    , m_MsgEngine(msgEngine)
{
    // Add default page
    m_pDefaultPage = new Page(*this);
    m_pDefaultPage->setGuideText(msgt("IDS_MSG_TMBODY_TEXT_MESSAGES"));
    append(*m_pDefaultPage);
}

Body::~Body()
{

}

std::string Body::getText() const
{
    // TODO: prepare message text
    return getDefaultPage().getPlainUtf8Text();
}

const Page &Body::getDefaultPage() const
{
    return *m_pDefaultPage;
}

Page &Body::getDefaultPage()
{
    return *m_pDefaultPage;
}

void Body::setListener(IBodyListener *listener)
{
    m_pListener = listener;
}


void Body::onCursorChanged(PageView &obj)
{

}

void Body::setFocus(bool focus)
{
    PageViewCollection pages = getPages();
    if(!pages.empty())
    {
        auto itEnd = pages.end();
        PageView *lastPage = *(--itEnd);
        lastPage->setFocus(true);
    }
}

void Body::clear()
{
    PageViewCollection pages = getPages();
    for(PageView *page : pages)
    {
        if(page == m_pDefaultPage)
        {
            page->clearText();
        }
        else
        {
            remove(*page);
        }
    }
}

BodySmsSize Body::getSmsSize() const
{
    BodySmsSize size;

    std::string text = getDefaultPage().getPlainUtf8Text();

    int textLen = m_MsgEngine.calculateTextLen(text);
    int maxChar = m_MsgEngine.getSettings().getMessageTextMaxChar();

    int txtLenTmp = textLen > 0 ? textLen - 1 : textLen;
    size.smsCount = txtLenTmp / maxChar + 1;
    size.charsLeft = maxChar - (textLen % (maxChar + 1));

    return size;
}

long long Body::getMmsSize() const
{
    return 0;
}


bool Body::isEmpty() const
{
    const PageViewCollection pages = getPages();
    for(PageView *page : pages)
    {
        if(!page->isEmpty())
        {
            return false;
        }
    }
    return true;
}

void Body::onFocused(PageView &obj)
{
    if(m_pListener)
        m_pListener->onFoucused(static_cast<Page&>(obj));
}

void Body::onUnfocused(PageView &obj)
{
    if(m_pListener)
        m_pListener->onUnfoucused(static_cast<Page&>(obj));
}

void Body::onChanged(PageView &obj)
{
    if(m_pListener)
        m_pListener->onChanged(static_cast<Page&>(obj));
}

void Body::onPreeditChanged(PageView &obj)
{
}

void Body::onPress(PageView &obj)
{

}

void Body::onClicked(PageView &obj)
{

}

void Body::onMaxLengthReached(PageView &obj)
{

}

void Body::onKeyDown(PageView &obj)
{

}

void Body::onKeyUp(PageView &obj)
{

}
