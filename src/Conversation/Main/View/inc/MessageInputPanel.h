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

#ifndef MessageInputPanel_h_
#define MessageInputPanel_h_

#include "View.h"

#include <string>

namespace Msg
{
    class IMessageInputPanelListener;

    class MessageInputPanel
        : public View
    {
        public:
            enum ButtonId
            {
                AddButtonId,
                SendButtonId
            };

        public:
            MessageInputPanel(Evas_Object *parent);
            virtual ~MessageInputPanel();

            void setEntry(Evas_Object *obj);
            void disabledButton(ButtonId id, bool val);
            void setCounter(const std::string &text);
            void setListener(IMessageInputPanelListener *l);

        private:
            void create(Evas_Object *parent);
            Evas_Object *createButton(Evas_Object *parent, const char *iconRes, const char *style, ButtonId buttonId);
            Evas_Object *getIcon(Evas_Object *button);
            Evas_Object *getButton(ButtonId id);

            void setNormalColor(Evas_Object *button);
            void setPressedColor(Evas_Object *button);
            void setDisabledColor(Evas_Object *button);

            void onButtonClicked(Evas_Object *obj, void *event_info);
            void onButtonPressed(Evas_Object *obj, void *event_info);
            void onButtonUnpressed(Evas_Object *obj, void *event_info);

        private:
            IMessageInputPanelListener * m_pListener;
            Evas_Object *m_pLayout;
            Evas_Object *m_pSendButton;
            Evas_Object *m_pAddButton;
    };

    class IMessageInputPanelListener
    {
        public:
            virtual ~IMessageInputPanelListener() {}
            virtual void onButtonClicked(MessageInputPanel &obj, MessageInputPanel::ButtonId id) {};
    };
}

#endif /* MessageInputPanel_h_ */
