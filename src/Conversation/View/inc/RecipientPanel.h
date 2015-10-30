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

#ifndef RecipientPanel_h_
#define RecipientPanel_h_

#include "View.h"
#include "RecipientViewItem.h"

#include <string>
#include <vector>

namespace Msg
{
    class IRecipientPanelListener;

    class RecipientPanel
        : public View
    {
        public:
            typedef std::vector<RecipientViewItem*> RecipientViewItemList;

        public:
            RecipientPanel(Evas_Object *parent, int entryMaxCharCount);
            virtual ~RecipientPanel();

            void setListener(IRecipientPanelListener *listener);
            void appendItem(RecipientViewItem &item);
            void showMbe(bool show);
            std::string getEntryText() const;
            RecipientViewItemList getItems() const;
            bool hasItems()const ;
            bool getEntryFocus() const;
            void setEntryFocus(bool val);
            void clearEntry();
            void setGuideText(const char *text);
            void setRecipientRect(Evas_Object *rect);

        private:
            void create(Evas_Object *parent);
            Evas_Object *mbeCreate(Evas_Object *parent);
            Evas_Object *entryCreate(Evas_Object *parent);
            Evas_Object *createContactBtn(Evas_Object *parent);
            void setContactBtnColor(int r, int g, int b, int a);
            RecipientViewItem *getItem(void *data);

        private:
            void onItemSelected(Evas_Object *obj, void *item);
            void onItemDeleted(Evas_Object *obj, void *item);
            void onItemClicked(Evas_Object *obj, void *item);
            void onItemPressed(Evas_Object *obj, void *item);

            Eina_Bool onMbeFilter(Evas_Object *obj, const char *item_label, const void *item_data);
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

            void onEntryGeometryChanged(Evas_Object *obj, void *event_info);

        private:
            IRecipientPanelListener *m_pListener;
            Evas_Object *m_pLayout;
            Evas_Object *m_pMbe;
            Evas_Object *m_pEntry;
            Evas_Object *m_pContactBtn;
            Evas_Object *m_pRecipRect;
            int m_EntryMaxCharCount;
    };

    class IRecipientPanelListener
    {
        public:
            virtual ~IRecipientPanelListener() {}

            virtual void onItemSelected(RecipientPanel &obj, RecipientViewItem &item) {};
            virtual void onItemPressed(RecipientPanel &obj, RecipientViewItem &item) {};
            virtual void onItemClicked(RecipientPanel &obj, RecipientViewItem &item) {};
            virtual void onKeyDown(RecipientPanel &obj, Evas_Event_Key_Down *ev) {};
            virtual void onEntryFocusChanged(RecipientPanel &obj) {};
            virtual void onContactButtonClicked(RecipientPanel &obj) {}
    };
}

#endif /* RecipientPanel_h_ */
