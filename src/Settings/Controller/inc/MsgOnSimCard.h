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

#ifndef MsgOnSimCard_h_h
#define MsgOnSimCard_h_h

#include "FrameController.h"
#include "HwButtonListener.h"
#include "ListView.h"
#include "SimListViewItem.h"
#include "Popup.h"
#include "ContextPopup.h"
#include "SelectAllListItem.h"

namespace Msg
{
    class NaviFrameController;
    class MsgSettings;

    class MsgOnSimCard
        : public FrameController
        , private IHwButtonListener
        , private IListViewListener
    {
        public:
            MsgOnSimCard(NaviFrameController &parent);
            virtual ~MsgOnSimCard();

        public:
            void showSelectAllItem(bool show, bool resetCheck = true);

        private:
            // NaviFrameItem:
            virtual void onAttached(ViewItem &item);
            virtual void onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId);
            void calcChecked(int &check, int &total) const;
            int getItemsCount() const;

            // IListViewListener:
            virtual void onListItemChecked(ListItem &listItem);
            void checkHandler(SelectAllListItem &item);

            // IHwButtonListener:
            virtual void onHwMoreButtonClicked();
            virtual void onHwBackButtonClicked();

            // ContextPopup callbacks:
            void onCopyToDeviceItemPressed(ContextPopupItem &item);
            void onDeleteItemPressed(ContextPopupItem &item);

        private:
            enum SimMode
            {
                NormalMode,
                CopyToDeviceMode,
                DeleteMode
            };

        private:
            void create();
            void showCopyDeletePopup();
            void setMode(SimMode mode);
            void setNormalMode();
            void setDeleteMode(bool value);
            void setCopyToDeviceMode(bool value);
            void setTitleTranslatable();
            void setTitleWithButtons(bool value);

        private:
            ListView *m_pList;
            SimMode m_SimMode;
            int m_CheckCount;
    };
}

#endif /* MsgOnSimCard_h_h */
