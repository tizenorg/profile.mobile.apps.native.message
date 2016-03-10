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

#ifndef BodyView_h_
#define BodyView_h_

#include "View.h"
#include "BodyViewItem.h"
#include "MediaPageViewItem.h"
#include "TextPageViewItem.h"
#include "ImagePageViewItem.h"
#include "SoundPageViewItem.h"
#include "VideoPageViewItem.h"
#include "BodyAttachmentViewItem.h"

#include <vector>

namespace Msg
{
    class PageView;
    class PageSeparator;

    typedef std::vector<PageView*> PageViewCollection;
    typedef std::vector<BodyAttachmentViewItem*> BodyAttachmentCollection;
    typedef std::vector<BodyViewItem*> BodyViewItemCollection;

    class BodyView
        : public View
        , private ITextPageViewItemListener
        , private IMediaPageViewItemListener
        , private IBodyAttachmentViewListener
    {
        friend class PageView;

        public:
            BodyView();
            virtual ~BodyView();

            void create(Evas_Object *parent);
            bool isEmpty() const;
            void clear();
            void setFocus(bool focus);
            const PageView &getDefaultPage() const;
            PageView &getDefaultPage();
            PageViewCollection getPages() const;
            int getPageCount() const;
            PageView *getNextPage(PageView &page) const;
            PageView *getPrevPage(PageView &page) const;
            PageView *addPage();
            BodyAttachmentCollection getAttachments() const;

        protected:
            virtual PageView &createPage() = 0;

            BodyAttachmentViewItem *addAttachment(const std::string &filePath, long long fileSize, const std::string &dispName = "");
            TextPageViewItem *addText(PageView &page);
            ImagePageViewItem *addImage(PageView &page, const std::string &filePath, long long fileSize);
            VideoPageViewItem *addVideo(PageView &page, const std::string &filePath, long long fileSize, const std::string &imagePath);
            SoundPageViewItem *addSound(PageView &page, const std::string &filePath, long long fileSize, const std::string &dispName = "");
            PageView *getPageForMedia(PageViewItem::Type type);
            void setFocus(PageView &page, bool focus);

        private:
            /*====Input signals====*/

            // ITextPageViewItemListener:
            virtual void onCursorChanged(TextPageViewItem &item);
            virtual void onFocused(TextPageViewItem &item);
            virtual void onUnfocused(TextPageViewItem &item);
            virtual void onPreeditChanged(TextPageViewItem &item);
            virtual void onPress(TextPageViewItem &item);
            virtual void onClicked(TextPageViewItem &item);
            virtual void onMaxLengthReached(TextPageViewItem &item);
            virtual void onKeyDown(TextPageViewItem &obj, Evas_Event_Key_Down &event);
            virtual void onKeyUp(TextPageViewItem &obj, Evas_Event_Key_Up &event);
            virtual void onChanged(TextPageViewItem &item);

            // IMediaPageViewItemListener:
            virtual void onDelete(PageViewItem &item);
            virtual void onPressed(MediaPageViewItem &item);
            virtual void onUnpressed(MediaPageViewItem &item);
            virtual void onFocused(MediaPageViewItem &item);
            virtual void onUnfocused(MediaPageViewItem &item);
            virtual void onKeyDown(MediaPageViewItem &item, Evas_Event_Key_Down &event);
            virtual void onKeyUp(MediaPageViewItem &item, Evas_Event_Key_Up &event);

            // IBodyAttachmentViewListener:
            virtual void onDelete(BodyAttachmentViewItem &item);

            /*====Output signals====*/
            virtual void onContentChanged() {};
            virtual void onItemDelete(PageViewItem &item) {};
            virtual void onItemDelete(BodyAttachmentViewItem &item) {};
            virtual void onClicked(BodyAttachmentViewItem &item) {};
            virtual void onClicked(MediaPageViewItem &item) {};

        private:
            template<typename T>
            std::vector<T*> getItems() const;
            BodyViewItemCollection getAllItems() const;
            int getItemCount(BodyViewItem::Type type) const;
            void showInputPanel(PageView &page, bool show);
            void showInputPanel(PageViewItem &pageItem, bool show);
            void removePage(PageView &page, bool setNextFocus);
            void removeAttachment(BodyAttachmentViewItem &attachment);
            void updateLastFocusedPage(PageViewItem &pageItem);
            void backKeyHandler(MediaPageViewItem &item);
            void backKeyHandler(TextPageViewItem &item);
            void clear(PageView &page);
            void prepare(BodyViewItem &item);
            void insertAfter(BodyViewItem &item, BodyViewItem &after);
            void insertBefore(BodyViewItem &item, BodyViewItem &before);
            void append(BodyViewItem &item);
            void remove(BodyViewItem &item);
            void rebuildSeparators();
            void updateSeparators();
            PageSeparator *createSep();
            void updateSep(PageSeparator &sep, int number, int maxNumber);

        private:
            BodyViewItemCollection m_Items;
            PageView *m_pDefaultPage;
            int m_LastTextCursorPos;
            PageView *m_pLastFocusedPage;
    };
}

#endif /* BodyView_h_ */
