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

#ifndef BubbleViewItem_h_
#define BubbleViewItem_h_

#include "View.h"
#include "MsgTypes.h"

#include <string>

namespace Msg
{
    class IBubbleViewItemListener;
    class BubbleEntity;

    class BubbleViewItem
        : public View
    {
        public:
            static const int maxWidth = 340;

            BubbleViewItem(BubbleEntity &entity);
            virtual ~BubbleViewItem();

            void setListener(IBubbleViewItemListener *l);
            BubbleEntity &getEntity();

        protected:
            void emitActionEvent();
            void attachGestureTapLayer(Evas_Object *parent, Evas_Object *obj);

        private:
            BubbleEntity &m_Entity;
            IBubbleViewItemListener *m_pListener;
    };

    class BubbleEntity
    {
        public:
            enum Type
            {
                TextItem,
                ImageItem,
                AudioItem,
                VideoItem,
                ContactItem,
                CalendarEventItem,
                UnknownFileItem,
                DownloadButtonItem
            };

        public:
            BubbleEntity(Type type);
            virtual ~BubbleEntity();

            Type getType() const;
            virtual const std::string &getFilePath() const = 0;
            virtual BubbleViewItem *createView(Evas_Object *parent) = 0;

        private:
            Type m_Type;
    };

    class IBubbleViewItemListener
    {
        public:
            virtual ~IBubbleViewItemListener() {};
            virtual void onAction(BubbleViewItem &item) {}; // Tap or Click
    };
}

#endif /* BubbleViewItem_h_ */
