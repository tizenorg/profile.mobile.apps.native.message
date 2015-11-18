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

#ifndef SettingFrame_h_
#define SettingFrame_h_

#include "FrameController.h"
#include "HwButtonListener.h"
#include "ListView.h"
#include "SettingsListItem.h"
#include "MsgSettings.h"

namespace Msg
{
    class NaviFrameController;
    class MsgSettings;
    class IMsgSettingsListener;

    class Settings
        : public FrameController
        , private IListViewListener
        , private ISettingsListViewItemListener
        , private IMsgSettingsListener
    {
        public:
            Settings(NaviFrameController &parent);
            virtual ~Settings();

        private:
            // NaviFrameItem:
            virtual void onAttached(ViewItem &item);
            virtual void onButtonClicked(const NaviFrameItem &item, NaviButtonId buttonId);

            // IListViewListener:
            virtual void onListItemSelected(ListItem &listItem, void *funcData);

            // ISettingsListViewItemListener
            virtual void onCheckButtonChanged(SettingsListViewItem &item);

            // IMsgSettingsListener
            virtual void onNotiSoundChanged(MsgSettings &msgSetting);

        private:
            MsgSettings &getMsgSettings();
            const MsgSettings &getMsgSettings() const;

            void setMmsAutoRetrRoamingBackup(bool value);
            bool getMmsAutoRetrRoamingBackup() const;
            bool isExistingMmsAutoRetrRoamingBackup() const;

            void create();
            void fillList();
            void updateItems();
            void appendGroupItem(const std::string &title);
            void callItemHandler(SettingsListItem &item);
            SettingsListItem *appendItem(ListItemStyleRef style,
                                         SettingsListItem::MethodHandler handler,
                                         const std::string &mainText,
                                         const std::string &subText = std::string());

            void appendAlertsItem();
            void appendSoundItem();
            void appendVibrationItem();
            void appendDelivReportSmsItem();
            void appendMsgOnSimItem();
            void appendDelivReportMmsItem();
            void appendReadReportItem();
            void appendAutoRetItem();
            void appendAutoRetRoamingItem();

            void updateAlertsItem();
            void updateSoundItem();
            void updateVibrationItem();
            void updateDelivReportSmsItem();
            void updateMsgOnSimItem();
            void updateDelivReportMmsItem();
            void updateReadReportItem();
            void updateAutoRetItem();
            void updateAutoRetRoamingItem();

            void alertsItemHandler(SettingsListItem &item);
            void soundItemHandler(SettingsListItem &item);
            void vibrationItemHandler(SettingsListItem &item);
            void delivReportSmsItemHandler(SettingsListItem &item);
            void msgOnSimItemHandler(SettingsListItem &item);
            void delivReportMmsItemHandler(SettingsListItem &item);
            void readReportItemHandler(SettingsListItem &item);
            void autoRetItemHandler(SettingsListItem &item);
            void autoRetRoamingItemHandler(SettingsListItem &item);

            bool isAutoRetRoamngDisabled() const;
            bool getAutoRetRoamng();

        private:
            ListView *m_pList;
            SettingsListItem *m_pAlertsItem;
            SettingsListItem *m_pSoundItem;
            SettingsListItem *m_pVibrationItem;
            SettingsListItem *m_pDelivReportSmsItem;
            SettingsListItem *m_pMsgOnSimItem;
            SettingsListItem *m_pDelivReportMmsItem;
            SettingsListItem *m_pReadReportItem;
            SettingsListItem *m_pAutoRetItem;
            SettingsListItem *m_pAutoRetRoamingItem;

    };
}

#endif /* SettingFrame_h_ */
