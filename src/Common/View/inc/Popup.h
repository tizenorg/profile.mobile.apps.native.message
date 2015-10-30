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

#ifndef Popup_h_
#define Popup_h_

#include "View.h"
#include "HwButtonListener.h"

#include <Evas.h>
#include <string>

namespace Msg
{
    class IPopupListener;
    class Popup;

    typedef void (*PopupCb)(Popup &popup, int buttonId, void *userData);

    class Popup
        : public View
        , private IHwButtonListener
    {
        public:
            enum ButtonID
            {
                OkButtonId,
                CancelButtonId
            };

        public:
            Popup(Evas_Object *parent);
            virtual ~Popup();

            void setUserType(int type);
            int getUserType() const;
            Evas_Object *getHostEvasObject() const;
            Evas_Object *setContent(Evas_Object *content);
            void setContent(const std::string &text);
            Evas_Object *getContent() const;
            void setHeight(int height);
            Evas_Object *addButton(const std::string &text, int buttonId, PopupCb popupCb = nullptr, void *userData = nullptr);
            void setListener(IPopupListener *listener);
            void setTitle(const std::string &title);

        private:
            // IHwButtonListener:
            virtual void onHwBackButtonClicked();
            virtual void onHwMoreButtonClicked();

            void create(Evas_Object *parent);

            static void on_button_clicked(void *data, Evas_Object *obj, void *event_info);

        private:
            Evas_Object *m_pBox;
            Evas_Object *m_pContent;
            int m_CurrentButtonIndex;
            IPopupListener *m_pListener;
            int m_Type;
    };

    class IPopupListener
    {
        public:
            virtual ~IPopupListener() {};
            virtual void onPopupButtonClicked(Popup &popup, int buttonId) {};
    };
}

#endif /* Popup_h_ */
