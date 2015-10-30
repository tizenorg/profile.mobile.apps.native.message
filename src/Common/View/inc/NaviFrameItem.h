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

#ifndef NaviFrameItem_h_
#define NaviFrameItem_h_

#include "ViewItem.h"

#include <string>
#include <memory>
#include <Elementary.h>

namespace Msg
{
    class NaviFrameView;

    class NaviFrameItem
        : public ViewItem
    {
        friend class NaviFrameView;

        public:
            enum NaviButtonId
            {
                NaviCancelButtonId = 0,
                NaviOkButtonId,
                NaviPrevButtonId,
                NaviButtonMax
            };

        public:
            class NaviBar;

            NaviFrameItem(NaviFrameView &owner);
            virtual ~NaviFrameItem();

        public:
            const NaviBar &getNaviBar() const;
            NaviBar &getNaviBar();
            NaviFrameView &getOwner();
            const NaviFrameView &getOwner() const;

        protected:
            virtual void onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId) {};

        private:
            NaviFrameView &m_Owner;
            NaviBar *m_pNaviBar;
    };

    class NaviFrameItem::NaviBar
    {
        friend class NaviFrameItem;

        public:
            NaviFrameItem &getOwner();
            const NaviFrameItem &getOwner() const;
            void show(bool value, bool transition = false);
            bool isVisible();
            void setTitle(const std::string &title);
            void setTitle(const TText &title);
            std::string getTitle() const;
            void setBadge(const std::string &badge);
            void showBadge(bool value);
            std::string getBadge() const;
            void showButton(NaviButtonId id, bool value);
            void disabledButton(NaviButtonId id, bool value);

        private:

            NaviBar(NaviFrameItem &onwer);
            ~NaviBar();

            void getButton(NaviButtonId id);
            void showCancelButtonPart(bool value);
            void showOkButtonPart(bool value);
            void setContent(Evas_Object * obj, const char *part);

            static void on_button_clicked(void *data, Evas_Object *obj, void *event_info);

        private:
            struct ButtonStruct
            {
                ButtonStruct(Evas_Object *b = nullptr, const char *p = nullptr, const char *s = nullptr)
                    : button(b)
                    , part(p)
                    , style(s)
                {
                }
                Evas_Object *button;
                const char *part;
                const char *style;
            };

        private:
            NaviFrameItem &m_Owner;
            std::string m_Badge;
            ButtonStruct ButtonList[NaviButtonMax];
    };
}

#endif /* NaviFrameItem_h_ */
