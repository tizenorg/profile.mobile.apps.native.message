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
#include "PageView.h"
#include "MsgEngine.h"
#include "FileUtils.h"
#include "TextPageViewItem.h"
#include "ImagePageViewItem.h"
#include "BodyMediaType.h"

#include <assert.h>

using namespace Msg;

namespace
{

    const std::string maxPageSepLabel = "3"; // TODO: check
    TextPageViewItem *getTextItem(PageView *page)
    {
        return page ? static_cast<TextPageViewItem*>(page->getItem(PageViewItem::TextType)) : nullptr;
    }
}

Body::Body(Evas_Object *parent, MsgEngine &msgEngine)
    : BodyView(parent)
    , m_pListener(nullptr)
    , m_MsgEngine(msgEngine)
{
    BodyView::setMaxPageLabel(maxPageSepLabel);
    m_pDefaultPage = addPage();
}

Body::~Body()
{
}

PageView *Body::addPage()
{
    PageView *page = new PageView(*this);
    TextPageViewItem *text = new TextPageViewItem(*page);
    text->setListener(this);
    text->setGuideText(msgt("IDS_MSG_TMBODY_TEXT_MESSAGES"));
    page->addItem(*text);
    BodyView::addPage(*page);
    return page;
}


void Body::setListener(IBodyListener *listener)
{
    m_pListener = listener;
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
    auto pages = getPages();
    for(PageView *page : pages)
    {
        if(page == m_pDefaultPage)
        {
            auto pageItems = page->getItems();
            for(PageViewItem *pageItem : pageItems)
            {
                PageViewItem::Type itemType = pageItem->getType();
                if(itemType != PageViewItem::TextType)
                {
                    page->removeItem(itemType);
                }
            }
        }
        else
        {
            page->destroy();
        }
    }
}

bool Body::isMms() const
{
    int pageCount = 0;
    auto bodyItems = BodyView::getAllItems();
    for(BodyViewItem *bodyItem : bodyItems)
    {
        BodyViewItem::Type type = bodyItem->getType();
        if(type == BodyViewItem::PageType)
        {
            ++pageCount;
            if(pageCount > 1 || isMms(static_cast<PageView&>(*bodyItem)))
            {
                return true;
            }
        }
        else if(type == BodyViewItem::AttachmentType)
        {
            return true;
        }
    }
    return false;
}

bool Body::isMms(const PageView &page) const
{
    auto pageItems = page.getItems();
    for(PageViewItem *pageItem : pageItems)
    {
        PageViewItem::Type itemType = pageItem->getType();
        if(itemType != PageViewItem::TextType)
        {
            return true;
        }
    }
    return false;
}

BodySmsSize Body::getSmsSize() const
{
    BodySmsSize size = {};
    TextPageViewItem *textItem = getTextItem(m_pDefaultPage);

    if(textItem)
    {
        std::string text = textItem->getPlainUtf8Text();

        int textLen = m_MsgEngine.calculateTextLen(text);
        int maxChar = m_MsgEngine.getSettings().getMessageTextMaxChar();

        int txtLenTmp = textLen > 0 ? textLen - 1 : textLen;
        size.smsCount = txtLenTmp / maxChar + 1;
        size.charsLeft = maxChar - (textLen % (maxChar + 1));
    }

    return size;
}

long long Body::getMmsSize() const
{
    return 0;
}

bool Body::isEmpty() const
{
    const auto pages = getPages();
    for(PageView *page : pages)
    {
        if(!page->isEmpty())
        {
            return false;
        }
    }
    return true;
}

bool Body::addMedia(const std::string &filePath)
{
    // If path is web uri then set path as body text:
    // TODO:

    // Check file path:
    if(!FileUtils::isExists(filePath) || FileUtils::isDir(filePath))
    {
        return false;
    }

    PageViewItem::Type itemType = getMediaType(filePath).type;
    if(itemType != PageViewItem::UnknownType)
    {
        PageViewItem *pageItem = nullptr;
        PageView *page = addPage();

        switch(itemType)
        {
            case PageViewItem::TextType:
            {
                break;
            }

            case PageViewItem::ImageType:
            {
                pageItem = new ImagePageViewItem(*page, filePath);
                break;
            }

            case PageViewItem::AudioType:
            {
                break;
            }

            case PageViewItem::VideoType:
            {
                break;
            }

            default:
                assert(false);
                break;
        }

        if(pageItem)
        {
            page->addItem(*pageItem);
        }
    }
    else
    {
        // TODO:
    }

    if(m_pListener)
        m_pListener->onChanged(*this);

    return true;
}

void Body::read(Message &msg)
{
    if(MessageSMS *sms = dynamic_cast<MessageSMS*>(&msg))
    {
        read(*sms);
    }
    else if(MessageMms *mms = dynamic_cast<MessageMms*>(&msg))
    {
        read(*mms);
    }
}

void Body::write(const Message &msg)
{
    if(const MessageSMS *sms = dynamic_cast<const MessageSMS*>(&msg))
    {
        write(*sms);
    }
    else if(const MessageMms *mms = dynamic_cast<const MessageMms*>(&msg))
    {
        write(*mms);
    }
}

void Body::write(const MessageSMS &msg)
{
    // TODO: impl
}

void Body::write(const MessageMms &msg)
{
    // TODO: impl
}

void Body::read(MessageSMS &msg)
{
    TextPageViewItem *textItem = getTextItem(m_pDefaultPage);
    assert(textItem);
    if(textItem)
    {
        msg.setText(textItem->getPlainUtf8Text());
    }
}

void Body::read(MessageMms &msg)
{
    auto pages = getPages();
    for(PageView *page : pages)
    {
        TextPageViewItem *textItem = getTextItem(page);
        if(textItem)
        {
            std::string textFile = m_WorkingDir.addTextFile(textItem->getPlainUtf8Text());
            MsgPage &msgPage = msg.addPage();
            MsgMedia &media = msgPage.addMedia();
            media.setType(MsgMedia::SmilText);
            media.setFilePath(textFile);
        }

        // TODO: add other SmilType and clear WorkingDir
    }
}

void Body::onChanged(TextPageViewItem &item)
{
    if(m_pListener)
        m_pListener->onChanged(*this);
}
