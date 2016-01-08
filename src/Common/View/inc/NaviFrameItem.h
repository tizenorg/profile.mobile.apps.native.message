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
#include "View.h"

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
                NaviCenterButtonId,
                NaviPrevButtonId,
                NaviDownButtonId,
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

            //ViewItem
            virtual void onAttached(ViewItem &item);

        private:
            void setTitleVisibility(bool visible);

        private:
            NaviFrameView &m_Owner;
            NaviBar *m_pNaviBar;
    };

    class NaviFrameItem::NaviBar
        : public View
    {
        friend class NaviFrameItem;

        public:
            enum NaviColorId
            {
                NaviBlueColorId = 0,
                NaviWhiteColorId,
                NaviColorMax
            };

        public:
            NaviFrameItem &getOwner();
            const NaviFrameItem &getOwner() const;
            void setVisible(bool visible);
            void setTitle(const std::string &title);
            void setTitle(const TText &title);
            std::string getTitle() const;
            void showButton(NaviButtonId id, bool value);
            void disabledButton(NaviButtonId id, bool value);
            void setButtonText(NaviButtonId id, const std::string &text);
            void setButtonText(NaviButtonId id, const TText &text);
            void clearBar();
            void setSearch(Evas_Object *searchPanel);
            Evas_Object *getSearch();
            const Evas_Object *getSearch() const;
            void showSearch();
            void hideSearch();
            void setColor(NaviColorId id);
            void expandDownButton(bool value);

        private:

            NaviBar(NaviFrameItem &onwer);
            virtual ~NaviBar();

            void getButton(NaviButtonId id);
            void setButtonColor(NaviButtonId id, NaviColorId titleColor);
            void showCancelButtonPart(bool value);
            void showOkButtonPart(bool value);
            void showCenterButtonPart(bool value);
            void showPrevButtonPart(bool value);
            void showDownButtonPart(bool value);
            void initNaviBar();

            static void on_button_clicked(void *data, Evas_Object *obj, void *event_info);

        private:
            struct ButtonStruct
            {
                ButtonStruct(Evas_Object *b = nullptr, const char *p = nullptr, const char *s = nullptr, const char *d = nullptr)
                    : button(b)
                    , part(p)
                    , style(s)
                    , default_text_id(d)
                {
                }
                Evas_Object *button;
                const char *part;
                const char *style;
                const char *default_text_id;
            };

        private:
            NaviFrameItem &m_Owner;
            std::string m_Badge;
            ButtonStruct m_ButtonList[NaviButtonMax];
            NaviColorId m_CurrentColor;
            Evas_Object *m_SearchBar;
    };
}

#endif /* NaviFrameItem_h_ */
