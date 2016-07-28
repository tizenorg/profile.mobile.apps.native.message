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
#include <vector>
#include <Elementary.h>

namespace Msg
{
    class NaviFrameView;
    class NaviFrameItem;
    typedef std::vector<NaviFrameItem*> NaviFrameItemList;

    /**
     * @brief Wraps all basic Naviframe-item operations.
     */
    class NaviFrameItem
        : public ViewItem
    {
        friend class NaviFrameView;

        public:
            /**
             * @brief An identifiers of buttons that can be shown on the top naviframe-item.
             */
            enum NaviButtonId
            {
                NaviCancelButtonId = 0,/**< Cancel-button */
                NaviOkButtonId,        /**< OK-button */
                NaviCenterButtonId,    /**< Central-button */
                NaviPrevButtonId,      /**< "Previous"-button */
                NaviExpandButtonId,    /**< "Expand"-button */
                NaviButtonMax
            };

        public:
            class NaviBar;

            /**
             * A constructor of NaviFrameItem object based on outside parent object. Creates nested navibar.
             * @param[in] owner an object responsible for automatic removing of NaviFrameItem instance.
             */
            NaviFrameItem(NaviFrameView &owner);
            virtual ~NaviFrameItem();

        public:
            /**
             * @brief Gets nested navibar.
             * @return nested navibar.
             */
            const NaviBar &getNaviBar() const;
            NaviBar &getNaviBar();

            /**
             * @brief Gets NaviframeView that owns this item.
             * @return owner of this item.
             */
            NaviFrameView &getOwner();
            const NaviFrameView &getOwner() const;

        protected:
            virtual void onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId) {};

            //ViewItem
            virtual void onAttached(ViewItem &item);
            virtual void onTransitionFinished(NaviFrameItem &item) {};

        private:
            void setTitleVisibility(bool visible);

        private:
            NaviFrameView &m_Owner;
            NaviBar *m_pNaviBar;
    };

    /**
     * @brief A bar at the top of NaviFrameItem.
     */
    class NaviFrameItem::NaviBar
        : public View
    {
        friend class NaviFrameItem;

        public:
            /**
             * @brief An enumeration of colors that could be applied to Navibar's background.
             */
            enum NaviColorId
            {
                NaviBlueColorId = 0,/**< Blue */
                NaviWhiteColorId,   /**< White */
                NaviColorMax
            };

        public:
            /**
             * @brief Gets NaviframeItem instance that owns this navibar
             * @return owner of this navibar.
             */
            NaviFrameItem &getOwner();
            const NaviFrameItem &getOwner() const;

            /**
             * @brief Manages navibar's visibility.
             * @param[in] visible if true shows navibar, if false - hides it.
             */
            void setVisible(bool visible);

            /**
             * @brief Sets navibar title with a specified plain text.
             * @param[in] title a plain text of new title.
             */
            void setTitle(const std::string &title);

            /**
             * @brief Sets Title with translatable string-id.
             * @param[in] title translatable text.
             */
            void setTitle(const TText &title);

            /**
             * @brief Gets navibar title text.
             * @return title.
             */
            std::string getTitle() const;

            /**
             * @brief Manages visibility of navibar-button identified with specified id.
             * @param[in] id button-id.
             * @param[in] value if true the button is visible, if false - hides the button.
             */
            void showButton(NaviButtonId id, bool value);

            /**
             * @brief Disables(enables) button with specified id.
             * @param[in] id id of button to be disabled(enabled).
             * @param[in] value if true disables the button otherwise enables it.
             */
            void disabledButton(NaviButtonId id, bool value);

            /**
             * @brief Sets specified plain text to a button with specified id.
             * @param[in] id button-id.
             * @param[in] text a plain text to set.
             */
            void setButtonText(NaviButtonId id, const std::string &text);

            /**
             * @brief Sets specified translatable text to a button with specified id.
             * @param[in] id button-id.
             * @param[in] text a translatable text to set.
             */
            void setButtonText(NaviButtonId id, const TText &text);

            /**
             * @brief Hides all buttons on navibar.
             */
            void clear();

            /**
             * @brief Sets search-panel layout.
             * @param[in] searchPanel a layout with search panel.
             */
            void setSearch(Evas_Object *searchPanel);

            /**
             * @brief Gets search-panel layout.
             * @return layout with search panel.
             */
            Evas_Object *getSearch();
            const Evas_Object *getSearch() const;

            /**
             * @brief Shows search-panel.
             */
            void showSearch();

            /**
             * @brief Hides search-panel.
             */
            void hideSearch();

            /**
             * @brief Changes navibar color.
             * @param[in] id code of supported color.
             */
            void setColor(NaviColorId id);

            /**
             * @brief Manages state of down-button.
             * @param Changes picture of down-button. If true sets down-arrow to down-button otherwise sets up-arrow there.
             */
            void setDownButtonState(bool expand);

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
            int getTextSize(NaviFrameItem::NaviButtonId id) const;

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
