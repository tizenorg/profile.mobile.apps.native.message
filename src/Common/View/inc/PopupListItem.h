/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef PopupListItem_h_
#define PopupListItem_h_

#include "ListItem.h"

namespace Msg
{
    class PopupList;
    class PopupListItem;
    typedef void (*PopupListItemPressedCb)(PopupListItem &item, void *userData);
    #define POPUPLIST_ITEM_PRESSED_CB(ClassName, method) [](PopupListItem &item, void *userData) \
    {                                                                                            \
        static_cast<ClassName*>(userData)->method(item);                                         \
    }

    /**
     * Generic Popup-list item class
     */
    class PopupListItem: public ListItem
    {
    public:
        PopupListItem(PopupList &parent, PopupListItemPressedCb cb, void *userData);
        PopupList &getParent();
        void fireCallback();
    private:
        PopupList &m_Parent;
        PopupListItemPressedCb m_Cb;
        void *m_pUserData;
    };

    /**
     * A class of popup-item with single text displayed
     */
    class PopupTextListItem: public PopupListItem
    {
    public:
        PopupTextListItem(PopupList &parent, const std::string &text, PopupListItemPressedCb cb, void *userData);
        virtual ~PopupTextListItem();
    private:
        virtual std::string getText(ListItem &item, const char *part);
    private:
        std::string m_Text;
    };

    /**
     * A class of popup-item with text and check field
     */
    class PopupCheckListItem: public PopupListItem
    {
        public:
            PopupCheckListItem(PopupList &parent, const std::string &text, PopupListItemPressedCb cb, void *userData);
            virtual ~PopupCheckListItem();

        private:
            virtual const char *getCheckPart(ListItem &item);
            virtual std::string getText(ListItem &item, const char *part);
        private:
            std::string m_Text;
    };
}

#endif /* PopupListItem_h_ */
