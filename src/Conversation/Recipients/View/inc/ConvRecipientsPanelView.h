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

#ifndef ConvRecipientsPanelView_h_
#define ConvRecipientsPanelView_h_

#include "View.h"

#include <string>
#include <vector>

namespace Msg
{
    class ConvRecipientsPanelView
        : public View
    {
        public:
            ConvRecipientsPanelView(Evas_Object *parent, int entryMaxCharCount);
            virtual ~ConvRecipientsPanelView();

            Evas_Object *getAreaRect() const;
            void showMbe(bool show);
            void showEntry(bool show);
            std::string getEntryText() const;
            void setEntryText(const std::string &utf8);
            bool isMbeEmpty() const;
            bool isMbeVisible() const;
            bool getEntryFocus() const;
            void setEntryFocus(bool val);
            void clearEntry();
            void clearMbe();
            void clear();

            /**
             * Gets a count of mbe items
             */
            unsigned int getItemsCount() const;

        protected:
            void setMbe(Evas_Object *obj);

        private:
            // Out signals:
            virtual void onKeyDown(Evas_Event_Key_Down *ev) {};
            virtual void onEntryFocusChanged() {};
            virtual void onContactButtonClicked() {}

        private:
            // In signals:
            void onItemSelected(Evas_Object *obj, void *item);
            void onItemDeleted(Evas_Object *obj, void *item);
            void onItemAdded(Evas_Object *obj, void *item);
            void onItemClicked(Evas_Object *obj, void *item);

            void onMbeFocused(Evas_Object *obj, void *event_info);
            void onMbeUnfocused(Evas_Object *obj, void *event_info);
            void onMbeClicked(Evas_Object *obj, void *event_info);

            void onEntryChanged(Evas_Object *obj, void *event_info);
            void onEntryPreeditChanged(Evas_Object *obj, void *event_info);
            void onEntryActivated(Evas_Object *obj, void *event_info);
            void onEntryFocusChanged(Evas_Object *obj, void *event_info);
            void onEntryClicked(Evas_Object *obj, void *event_info);
            void onEntryMaxlengthReached(Evas_Object *obj, void *event_info);
            void onKeyDown(Evas_Object *obj, void *event_info);

            void onContactBtnPressed(Evas_Object *obj, void *event_info);
            void onContactBtnUnpressed(Evas_Object *obj, void *event_info);
            void onContactBtnClicked(Evas_Object *obj, void *event_info);

            void onGeometryChanged(Evas_Object *obj, void *event_info);

        private:
            void create(Evas_Object *parent);
            Evas_Object *createEntry(Evas_Object *parent);
            Evas_Object *createAreaRect(Evas_Object *parent);
            Evas_Object *createContactBtn(Evas_Object *parent);
            void setContactBtnColor(int r, int g, int b, int a);
            void deleteNextRecipient();
            void selectLastItem();
            bool isEntryEmpty() const;
            void unselectMbeItem();
            void addGeometryChangedCb(Evas_Object *obj);

        private:
            Evas_Object *m_pLayout;
            Evas_Object *m_pMbe;
            Evas_Object *m_pEntry;
            Evas_Object *m_pContactBtn;
            Evas_Object *m_pRect;
            int m_EntryMaxCharCount;
            bool m_IsMbeVisible;
    };
}

#endif /* ConvRecipientsPanelView_h_ */
