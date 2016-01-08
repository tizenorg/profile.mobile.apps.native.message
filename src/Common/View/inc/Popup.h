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
#include "LangUtils.h"

#include <Evas.h>
#include <string>

namespace Msg
{
    class IPopupListener;
    class Popup;
    class PopupManager;

    typedef void (*PopupButtonCb)(Popup &popup, int buttonId, void *userData);
    #define POPUP_BUTTON_CB(ClassName, method) [](Popup &popup, int buttonId, void *userData) \
    {                                                                                         \
        static_cast<ClassName*>(userData)->method(popup, buttonId);                           \
    }

    class Popup
        : public View
    {
        public:

            static void defaultButtonCb(Popup &popup, int buttonId, void *userData);
            enum ButtonID
            {
                OkButtonId,
                CancelButtonId
            };

        public:
            Popup(Evas_Object *parent);
            Popup(PopupManager &parent);
            virtual ~Popup();

            Evas_Object *getHostEvasObject() const;
            Evas_Object *setContent(Evas_Object *content);
            void setContent(const std::string &text);
            void setContent(const TText &text);
            Evas_Object *getContent() const;
            void setHeight(int height);
            Evas_Object *addButton(const TText &text, int buttonId, PopupButtonCb buttonCb = defaultButtonCb, void *userData = nullptr);
            void setTitle(const std::string &title);
            void setTitle(const TText &title);
            void destroy();

        private:

            void create(Evas_Object *parent);
            static void on_button_clicked(void *data, Evas_Object *obj, void *event_info);

        private:
            Evas_Object *m_pBox;
            Evas_Object *m_pContent;
            int m_CurrentButtonIndex;
            PopupManager *m_pManager;
    };
}

#endif /* Popup_h_ */
