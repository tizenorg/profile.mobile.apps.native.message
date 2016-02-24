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

#ifndef MsgThreadSearchPanel_h_
#define MsgThreadSearchPanel_h_

#include "View.h"
#include <string>

namespace Msg
{
    class IMsgThreadSearchPanelListener;

    class MsgThreadSearchPanel
        : public View
    {
        public:
            MsgThreadSearchPanel(Evas_Object *parent);
            virtual ~MsgThreadSearchPanel();

            void setListener(IMsgThreadSearchPanelListener *listener);
            std::string getEntryText() const;
            void clearEntry();
            void showClearButton(bool visible);
            void setGuideText(const std::string &text);
            void setEntryFocus(bool focus);
            bool isEmpty() const;

        private:
            void create(Evas_Object *parent);
            Evas_Object *createClearButton(Evas_Object *parent);
            Evas_Object *createEntry(Evas_Object *parent);
            Evas_Object *createButton(Evas_Object *parent, const char *style);

        private:
            void onEntryChanged(Evas_Object *obj, void *eventInfo);
            void onKeyDown(Evas_Object *obj, void *eventInfo);
            void onClearButtonClicked(Evas_Object *obj, void *eventInfo);

        private:
            Evas_Object *m_pEntry;
            Evas_Object *m_pClearButton;
            IMsgThreadSearchPanelListener *m_pListener;
    };

    class IMsgThreadSearchPanelListener
    {
        public:
            virtual ~IMsgThreadSearchPanelListener() {};

            virtual void onBackButtonClicked(MsgThreadSearchPanel &obj) {};
            virtual void onSearchButtonClicked(MsgThreadSearchPanel &obj) {};
            virtual void onEntryChanged(MsgThreadSearchPanel &obj) {};
    };
}

#endif // MsgThreadSearchPanel_h_
