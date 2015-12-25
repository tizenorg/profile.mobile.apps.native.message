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
#include "FileUtils.h"
#include "TextPageViewItem.h"
#include "ImagePageViewItem.h"
#include "BodyMediaType.h"
#include "MsgEngine.h"
#include "Logger.h"

#include <assert.h>
#include <string.h>

using namespace Msg;

namespace
{
    inline TextPageViewItem *getTextItem(const PageView &page)
    {
        return page ? static_cast<TextPageViewItem*>(page.getItem(PageViewItem::TextType)) : nullptr;
    }
}

Body::Body(Evas_Object *parent, MsgEngine &msgEngine)
    : BodyView(parent)
    , m_pListener(nullptr)
    , m_MsgEngine(msgEngine)
    , m_pOnChangedIdler(nullptr)
{
}

Body::~Body()
{
    if(m_pOnChangedIdler)
    {
        ecore_idler_del(m_pOnChangedIdler);
        m_pOnChangedIdler = nullptr;
    }
}

void Body::setListener(IBodyListener *listener)
{
    m_pListener = listener;
}

bool Body::addMedia(const std::list<std::string> &fileList)
{
    bool res = true;
    for(auto &file : fileList)
        res &= addMedia(file);
    return res;
}

bool Body::addMedia(const std::string &filePath)
{
    MSG_LOG("Try add resource:", filePath);

    // TODO:
    // If path is web uri then set path as body text:
    // Check file path:
    // Use working dir:

    if(!FileUtils::isExists(filePath) || FileUtils::isDir(filePath))
    {
        MSG_LOG_ERROR("File not exists: ", filePath);
        return false;
    }

    bool res = BodyView::addMedia(filePath);

    if(m_pListener)
        m_pListener->onChanged(*this);

    return res;
}

bool Body::isMms() const
{
    auto pages = getPages();

    if(pages.size() > 1)
        return true;

    for(PageView *page : pages)
    {
        if(isMms(*page))
            return true;
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
            return true;
    }
    return false;
}

BodySmsSize Body::getSmsSize() const
{
    BodySmsSize size = {};
    TextPageViewItem *textItem = getTextItem(getDefaultPage());

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

void Body::read(Message &msg)
{
    if(MessageSMS *sms = dynamic_cast<MessageSMS*>(&msg))
        read(*sms);
    else if(MessageMms *mms = dynamic_cast<MessageMms*>(&msg))
        read(*mms);
}

void Body::write(const Message &msg)
{
    if(const MessageSMS *sms = dynamic_cast<const MessageSMS*>(&msg))
        write(*sms);
    else if(const MessageMms *mms = dynamic_cast<const MessageMms*>(&msg))
        write(*mms);
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
    TextPageViewItem *textItem = getTextItem(getDefaultPage());
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
        TextPageViewItem *textItem = getTextItem(*page);
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

void Body::onContentChanged()
{
    if(!m_pOnChangedIdler)
    {
        m_pOnChangedIdler = ecore_idler_add
        (
            [](void *data)->Eina_Bool
            {
                Body *self =(Body*)data;
                if(self->m_pListener)
                    self->m_pListener->onChanged(*self);
                self->m_pOnChangedIdler = nullptr;
                return false; // Delete idler
            },
            this
        );
    }
}
