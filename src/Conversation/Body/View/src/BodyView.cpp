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

#include "BodyView.h"
#include "PageView.h"
#include "PageSeparator.h"
#include "Logger.h"
#include "BodyMediaType.h"

#include <assert.h>
#include <algorithm>

using namespace Msg;

namespace
{
    inline bool isBackKey(const char *name)
    {
        return name ? strcmp("BackSpace", name) == 0 : false;
    }
}

BodyView::BodyView()
    : m_pDefaultPage(nullptr)
    , m_LastTextCursorPos(0)
    , m_pLastFocusedPage(nullptr)
{
}

BodyView::~BodyView()
{

}

void BodyView::create(Evas_Object *parent)
{
    setEo(elm_box_add(parent));
    expand();
    m_pDefaultPage = addPage();
}

const PageView &BodyView::getDefaultPage() const
{
    assert(m_pDefaultPage);
    return *m_pDefaultPage;
}

PageView &BodyView::getDefaultPage()
{
    assert(m_pDefaultPage);
    return *m_pDefaultPage;
}

bool BodyView::isEmpty() const
{
    const auto pages = getPages();

    if(pages.size() > 1)
        return false;

    for(PageView *page : pages)
    {
        if(!page->isEmpty())
            return false;
    }

    return getAttachments().empty();
}

PageView *BodyView::getNextPage(PageView &page) const
{
    auto pages = getPages();
    auto itEnd = pages.end();
    auto it = std::find(pages.begin(), pages.end(), &page);

    if(it != itEnd)
        ++it;

    return it != itEnd ? *it : nullptr;
}

PageView *BodyView::getPrevPage(PageView &page) const
{
    auto pages = getPages();

    auto itBegin = pages.begin();
    auto itEnd = pages.end();
    auto it = std::find(pages.begin(), pages.end(), &page);

    if(it != itEnd && it != itBegin)
        --it;

    return it != itEnd ? *it : nullptr;
}

void BodyView::setMaxPageLabel(const std::string &max)
{
    m_MaxPageLabel = max;
}

int BodyView::getItemCount(BodyViewItem::Type type) const
{
    int count = 0;
    auto list = getAllItems();
    for(BodyViewItem *item : list)
    {
        if(item->getType() == type)
            ++count;
    }
    return count;
}

PageSeparator *BodyView::createSep(int number)
{
    PageSeparator *sep = new PageSeparator(*this);
    sep->setText(std::to_string(number) + '/' + m_MaxPageLabel);
    return sep;
}

void BodyView::rebuildPageSeparators()
{
    auto separators = getItems<PageSeparator>();
    for(PageSeparator *sep : separators)
        remove(*sep);

    auto pages = getPages();
    PageView *prevPage = nullptr;
    int number = 0;

    for(PageView *page : pages)
    {
        if(prevPage)
        {
            ++number;
            PageSeparator *sep = createSep(number);
            insertAfter(*sep, *prevPage);
        }
        prevPage = page;
    }
}

void BodyView::prepare(BodyViewItem &item)
{
    expand(item);
    item.show();
}

void BodyView::append(BodyViewItem &item)
{
    prepare(item);
    elm_box_pack_end(getEo(), item);
    item.show();
    onContentChanged();
}

void BodyView::insertAfter(BodyViewItem &item, BodyViewItem &after)
{
    prepare(item);
    elm_box_pack_after(getEo(), item, after);
    onContentChanged();
}

void BodyView::insertBefore(BodyViewItem &item, BodyViewItem &before)
{
    prepare(item);
    elm_box_pack_before(getEo(), item, before);
    onContentChanged();
}

void BodyView::remove(BodyViewItem &item)
{
    elm_box_unpack(getEo(), item);
    item.destroy();
    onContentChanged();
}

int BodyView::getPageCount() const
{
    return getPages().size();
}

PageViewCollection BodyView::getPages() const
{
    return getItems<PageView>();
}

BodyAttachmentCollection BodyView::getAttachments() const
{
    return getItems<BodyAttachmentViewItem>();
}

template<typename T>
std::vector<T*> BodyView::getItems() const
{
    std::vector<T*> res;
    BodyViewItemCollection all = getAllItems();
    for(BodyViewItem *item : all)
    {
        if(T *itemT = dynamic_cast<T*>(item))
            res.push_back(itemT);
    }
    return res;
}

BodyViewItemCollection BodyView::getAllItems() const
{
    BodyViewItemCollection res;
    Eina_List *list = elm_box_children_get(getEo());
    if(list)
    {
        Eina_List *l = nullptr;
        void *obj = nullptr;

        EINA_LIST_FOREACH(list, l, obj)
        {
            BodyViewItem *item = reinterpretCast<BodyViewItem*>(obj);
            if(item)
                res.push_back(item);
        }
        eina_list_free(list);
    }

    return res;
}

PageView *BodyView::addPage()
{
    PageView &page = createPage();

    int pageCount = getItemCount(BodyViewItem::PageType);
    if(pageCount > 0)
        append(*createSep(pageCount));

    append(page);
    return &page;
}

BodyAttachmentViewItem *BodyView::addAttachment(const std::string &filePath, long long fileSize, const std::string &dispName)
{
    BodyAttachmentViewItem *attachment = new BodyAttachmentViewItem(*this, filePath, fileSize, dispName);
    insertBefore(*attachment, *m_pDefaultPage);
    attachment->setListener(this);
    onContentChanged();
    return attachment;
}

void BodyView::setFocus(PageView &page, bool focus)
{
    TextPageViewItem *textItem = static_cast<TextPageViewItem*>(page.getItem(PageViewItem::TextType));
    if(textItem)
    {
        textItem->setFocus(focus);
        textItem->setEndCursorPos();
    }
}

void BodyView::setFocus(bool focus)
{
    setFocus(*m_pDefaultPage, focus);
}

void BodyView::clear(PageView &page)
{
    auto pageItems = page.getItems();
    for(PageViewItem *pageItem : pageItems)
    {
        if(pageItem->getType() == PageViewItem::TextType)
            static_cast<TextPageViewItem*>(pageItem)->clear();
        else
            page.removeItem(*pageItem);
    }
}

void BodyView::clear()
{
    auto items = getAllItems();
    for(BodyViewItem *item : items)
    {
        if(item->getType() == BodyViewItem::PageType)
        {
            PageView *page = static_cast<PageView*>(item);
            if(page == m_pDefaultPage)
                clear(*page);
            else
                removePage(*page, false);
        }
        else if(item->getType() == BodyViewItem::AttachmentType)
        {
            BodyAttachmentViewItem *attachment = static_cast<BodyAttachmentViewItem*>(item);
            removeAttachment(*attachment);
        }
    }
}

void BodyView::showInputPanel(PageView &page, bool show)
{
    TextPageViewItem *item = static_cast<TextPageViewItem*>(page.getItem(PageViewItem::TextType));
    if(item)
        item->showInputPanel(show);
}

void BodyView::showInputPanel(PageViewItem &pageItem, bool show)
{
    if(pageItem.getType() == PageViewItem::TextType)
        static_cast<TextPageViewItem&>(pageItem).showInputPanel(show);
    else
        showInputPanel(pageItem.getParentPage(), show);
}

ImagePageViewItem *BodyView::addImage(PageView &page, const std::string &filePath, long long fileSize)
{
    ImagePageViewItem *item = new ImagePageViewItem(page, filePath, fileSize, filePath);
    item->setListener(this);
    item->show();
    page.addItem(*item);
    onContentChanged();
    return item;
}

SoundPageViewItem *BodyView::addSound(PageView &page, const std::string &filePath, long long fileSize, const std::string &dispName)
{
    SoundPageViewItem *item = new SoundPageViewItem(page, filePath, fileSize, dispName);
    item->setListener(this);
    item->show();
    page.addItem(*item);
    onContentChanged();
    return item;
}

VideoPageViewItem *BodyView::addVideo(PageView &page, const std::string &filePath, long long fileSize, const std::string &imagePath)
{
    VideoPageViewItem *item = new VideoPageViewItem(page, filePath, fileSize, imagePath);
    item->setListener(this);
    item->show();
    page.addItem(*item);
    onContentChanged();
    return item;
}

TextPageViewItem *BodyView::addText(PageView &page)
{
    TextPageViewItem *item = new TextPageViewItem(page);
    item->setListener(this);
    item->show();
    item->setGuideText(msgt("IDS_MSG_TMBODY_TEXT_MESSAGES"));
    page.addItem(*item);
    onContentChanged();
    return item;
}

void BodyView::updateLastFocusedPage(PageViewItem &pageItem)
{
    m_pLastFocusedPage = &pageItem.getParentPage();
}

PageView *BodyView::getPageForMedia(PageViewItem::Type type)
{
    PageView *page = m_pLastFocusedPage;

    if(page && page->canItemBeAdded(type))
        return page;

    page = addPage();

    return page;
}

void BodyView::removePage(PageView &page, bool setNextFocus)
{
    if(m_pDefaultPage != &page && getPageCount() > 1)
    {
        if(setNextFocus)
        {
            PageView *prevPage = getPrevPage(page);
            if(prevPage)
                setFocus(*prevPage, true);
        }

        page.View::destroy();
        rebuildPageSeparators();

        if(m_pLastFocusedPage == &page)
            m_pLastFocusedPage = nullptr;
    }
}

void BodyView::removeAttachment(BodyAttachmentViewItem &attachment)
{
    remove(attachment);
}

void BodyView::backKeyHandler(MediaPageViewItem &item)
{
    item.destroy();
}

void BodyView::backKeyHandler(TextPageViewItem &item)
{
    if(m_LastTextCursorPos == 0)
    {
        MSG_LOG("Last text cursor position");
        PageView &page = item.getParentPage();

        PageViewItem *topItem = page.getItem(PageViewItem::ImageType);
        if(!topItem)
            topItem = page.getItem(PageViewItem::VideoType);

        if(topItem)
            topItem->setFocus(true);
        else
            removePage(page, true);
    }
}

void BodyView::onCursorChanged(TextPageViewItem &item)
{
    m_LastTextCursorPos = item.getCursorPos();
    MSG_LOG("Cursor pos: ", item.getCursorPos());
}

void BodyView::onFocused(TextPageViewItem &item)
{
    MSG_LOG("");
    updateLastFocusedPage(item);
    item.showInputPanel(true);
}

void BodyView::onUnfocused(TextPageViewItem &item)
{
    MSG_LOG("");
    item.showInputPanel(false);
}

void BodyView::onPreeditChanged(TextPageViewItem &item)
{
    MSG_LOG("");
}

void BodyView::onPress(TextPageViewItem &item)
{
    MSG_LOG("");
}

void BodyView::onClicked(TextPageViewItem &item)
{
    MSG_LOG("");
    showInputPanel(item, true);
}

void BodyView::onMaxLengthReached(TextPageViewItem &item)
{
    MSG_LOG("");
}

void BodyView::onKeyDown(TextPageViewItem &item, Evas_Event_Key_Down &event)
{
    MSG_LOG("");
    if(isBackKey(event.keyname))
        backKeyHandler(item);
}

void BodyView::onKeyUp(TextPageViewItem &item, Evas_Event_Key_Up &event)
{
    MSG_LOG("");
}

void BodyView::onChanged(TextPageViewItem &item)
{
    MSG_LOG("");
    onContentChanged();
}

void BodyView::onPressed(MediaPageViewItem &item)
{
    MSG_LOG("");
}

void BodyView::onUnpressed(MediaPageViewItem &item)
{
    MSG_LOG("");
}

void BodyView::onFocused(MediaPageViewItem &item)
{
    MSG_LOG("");
    updateLastFocusedPage(item);
    item.highlight(true);
    showInputPanel(item, true);
}

void BodyView::onUnfocused(MediaPageViewItem &item)
{
    MSG_LOG("");
    item.highlight(false);
    showInputPanel(item, false);
}

void BodyView::onKeyDown(MediaPageViewItem &item, Evas_Event_Key_Down &event)
{
    MSG_LOG("");
    if(isBackKey(event.keyname))
        backKeyHandler(item);
}

void BodyView::onKeyUp(MediaPageViewItem &item, Evas_Event_Key_Up &event)
{
    MSG_LOG("");
}

void BodyView::onDelete(BodyAttachmentViewItem &item)
{
    MSG_LOG("");
    onItemDelete(item);
    onContentChanged();
}

void BodyView::onDelete(PageViewItem &item)
{
    MSG_LOG("");
    onItemDelete(item);
    onContentChanged();
}
