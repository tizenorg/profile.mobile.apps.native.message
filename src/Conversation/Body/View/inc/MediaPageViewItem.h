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

#ifndef MediaPageViewItem_h_
#define MediaPageViewItem_h_

#include "PageViewItem.h"

#include <vector>

namespace Msg
{
    class IMediaPageViewItemListener;

    class MediaPageViewItem
        : public PageViewItem
    {
        public:
            MediaPageViewItem(PageView &parent, const std::string &resourcePath);
            virtual ~MediaPageViewItem();

            void setListener(IMediaPageViewItemListener *l);
            const std::string &getResourcePath() const;

        protected:
            Evas_Object *getMediaLayout() const;
            Evas_Object *getButtonLayout() const;
            void setButtonContent(Evas_Object *layout);
            void setRect(Evas_Object *layout);

        private:
            typedef void (IMediaPageViewItemListener::*ListenerMethod)(MediaPageViewItem &);

            Evas_Object *createMainLayout(Evas_Object *parent);
            Evas_Object *createButton(Evas_Object *parent);
            Evas_Object *createRect(Evas_Object *parent);
            Evas_Object *createGestureLayer(Evas_Object *parent);
            static void notifyListener(void *data, ListenerMethod method);

        private:
            IMediaPageViewItemListener *m_pListener;
            Evas_Object *m_pButton;
            const std::string m_ResourcePath;
    };

    class IMediaPageViewItemListener
    {
        public:
            virtual ~IMediaPageViewItemListener() {}

            virtual void onClicked(MediaPageViewItem &item) {};
            virtual void onPressed(MediaPageViewItem &item) {};
            virtual void onUnpressed(MediaPageViewItem &item) {};
            virtual void onFocused(MediaPageViewItem &item) {};
            virtual void onUnfocused(MediaPageViewItem &item) {};
    };
}

#endif /* MediaPageViewItem_h_ */
