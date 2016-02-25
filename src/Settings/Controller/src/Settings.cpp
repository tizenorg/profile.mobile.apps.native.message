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

#include "Settings.h"
#include "SettingsSound.h"
#include "Config.h"
#include "MsgEngine.h"
#include "Logger.h"
#include "PathUtils.h"
#include "SettingsGroupListViewItem.h"
#include "SettingsListItem.h"
#include "Logger.h"
#include "MsgOnSimCard.h"
#include "MediaUtils.h"

#include <Elementary.h>
#include <app_preference.h>
#include <sstream>

using namespace Msg;

#define KEY_AUTO_RETRIEVE_WHILE_ROAMING_BACKUP_KEY PREF_PREFX"/auto_retrieve_while_roaming_backup"

Settings::Settings(NaviFrameController &parent)
    : FrameController(parent)
    , m_pList(nullptr)
    , m_pAlertsItem(nullptr)
    , m_pSoundItem(nullptr)
    , m_pVibrationItem(nullptr)
    , m_pDelivReportSmsItem(nullptr)
    , m_pMsgOnSimItem(nullptr)
    , m_pDelivReportMmsItem(nullptr)
    , m_pReadReportItem(nullptr)
    , m_pAutoRetItem(nullptr)
    , m_pAutoRetRoamingItem(nullptr)
{
    create();
}

Settings::~Settings()
{
     getMsgSettings().removeListener(*this);
}

void Settings::create()
{
    if(!isExistingMmsAutoRetrRoamingBackup())
        setMmsAutoRetrRoamingBackup(getMsgSettings().getMmsAutoRetrRoaming());

    m_pList = new ListView(getParent());
    m_pList->setListener(this);
    m_pList->expand();
    m_pList->setMultiSelection(false);
    m_pList->show();
    m_pList->setMode(ELM_LIST_COMPRESS);
    fillList();
    updateItems();
    getMsgSettings().addListener(*this);
}

MsgSettings &Settings::getMsgSettings()
{
    return getMsgEngine().getSettings();
}

const MsgSettings &Settings::getMsgSettings() const
{
    return getMsgEngine().getSettings();
}

void Settings::setMmsAutoRetrRoamingBackup(bool value)
{
    preference_set_boolean(KEY_AUTO_RETRIEVE_WHILE_ROAMING_BACKUP_KEY, value);
}

bool Settings::getMmsAutoRetrRoamingBackup() const
{
    bool value = false;
    preference_get_boolean(KEY_AUTO_RETRIEVE_WHILE_ROAMING_BACKUP_KEY, &value);
    return value;
}

bool Settings::isExistingMmsAutoRetrRoamingBackup() const
{
    bool value = false;
    preference_is_existing(KEY_AUTO_RETRIEVE_WHILE_ROAMING_BACKUP_KEY, &value);
    return value;
}

void Settings::fillList()
{
    appendGroupItem("Notifications");
    appendAlertsItem();
    appendSoundItem();
    appendVibrationItem();

    appendGroupItem("Text messages");
    appendDelivReportSmsItem();
    appendMsgOnSimItem();

    appendGroupItem("Multimedia message");
    appendDelivReportMmsItem();
    appendReadReportItem();
    appendAutoRetItem();
    appendAutoRetRoamingItem();
}

void Settings::updateItems()
{
    updateAlertsItem();
    updateSoundItem();
    updateVibrationItem();
    updateDelivReportSmsItem();
    updateMsgOnSimItem();
    updateDelivReportMmsItem();
    updateReadReportItem();
    updateAutoRetItem();
    updateAutoRetRoamingItem();
}

void Settings::appendGroupItem(const std::string &title)
{
    SettingsGroupListViewItem *item = new SettingsGroupListViewItem(title);
    m_pList->appendItem(*item);
}

SettingsListItem *Settings::appendItem(ListItemStyleRef style, SettingsListItem::MethodHandler handler,
                                       const std::string &mainText, const std::string &subText)
{
    SettingsListItem *item = new SettingsListItem(style, handler);
    item->setListener(this);
    item->setMainText(mainText);
    item->setSubText(subText);
    m_pList->appendItem(*item);
    return item;
}

void Settings::appendAlertsItem()
{
    m_pAlertsItem = appendItem(SettingsListViewItem::oneLineIconStyle, &Settings::alertsItemHandler, "Alerts");
}

void Settings::appendSoundItem()
{
    m_pSoundItem = appendItem(SettingsListViewItem::multiLineStyle, &Settings::soundItemHandler, "Sound", "Default");
}

void Settings::appendVibrationItem()
{
    m_pVibrationItem = appendItem(SettingsListViewItem::oneLineIconStyle, &Settings::vibrationItemHandler, "Vibration");
}

void Settings::appendDelivReportSmsItem()
{
    m_pDelivReportSmsItem = appendItem
    (   SettingsListViewItem::multiLineIconStyle,
         &Settings::delivReportSmsItemHandler,
        "Delivery report",
        "Request a delivery report for each message you send"
    );
}

void Settings::appendMsgOnSimItem()
{
    m_pMsgOnSimItem = appendItem
    (
        SettingsListViewItem::multiLineStyle,
        &Settings::msgOnSimItemHandler,
        dgettext(PROJECT_NAME, "IDS_MSG_TMBODY_MESSAGES_ON_SIM_CARD"),
        dgettext(PROJECT_NAME, "IDS_MSG_SBODY_MANAGE_THE_MESSAGES_STORED_ON_YOUR_SIM_CARD")
    );
}

void Settings::appendDelivReportMmsItem()
{
    m_pDelivReportMmsItem = appendItem
    (
        SettingsListViewItem::multiLineIconStyle,
        &Settings::delivReportMmsItemHandler,
        "Delivery report",
        "Request a delivery report for each message you send"
    );
}

void Settings::appendReadReportItem()
{
    m_pReadReportItem = appendItem
    (
        SettingsListViewItem::multiLineIconStyle,
        &Settings::readReportItemHandler,
        "Read report",
        "Request a read report for each message you send"
    );
}

void Settings::appendAutoRetItem()
{
    m_pAutoRetItem = appendItem
    (
        SettingsListViewItem::multiLineIconStyle,
        &Settings::autoRetItemHandler,
        "Auto retrieve",
        "Request message automatically"
    );
}

void Settings::appendAutoRetRoamingItem()
{
    m_pAutoRetRoamingItem = appendItem
    (
         SettingsListViewItem::multiLineIconStyle,
         &Settings::autoRetRoamingItemHandler,
         "Auto retrieve while roaming",
         "Retrieve messages automatically while roaming"
    );
}

void Settings::updateAlertsItem()
{
    m_pAlertsItem->setCheckButtonState(getMsgSettings().getAlerts());
}

void Settings::updateSoundItem()
{
    MSG_LOG("updateSoundItem activated!");
    m_pSoundItem->disabled(!getMsgSettings().getAlerts());
    m_pSoundItem->setSubText(MediaUtils::getTitle(getMsgSettings().getNotiSound()));
}

void Settings::updateVibrationItem()
{
    m_pVibrationItem->disabled(!getMsgSettings().getAlerts());
    m_pVibrationItem->setCheckButtonState(getMsgSettings().getVibration());
}

void Settings::updateDelivReportSmsItem()
{
    m_pDelivReportSmsItem->setCheckButtonState(getMsgSettings().getSmsDelivReport());
}

void Settings::updateMsgOnSimItem()
{
    // TODO:
}

void Settings::updateDelivReportMmsItem()
{
    m_pDelivReportMmsItem->setCheckButtonState(getMsgSettings().getMmsDelivReport());
}

void Settings::updateReadReportItem()
{
    m_pReadReportItem->setCheckButtonState(getMsgSettings().getMmsReadReport());
}

void Settings::updateAutoRetItem()
{
    m_pAutoRetItem->setCheckButtonState(getMsgSettings().getMmsAutoRetr());
}

void Settings::updateAutoRetRoamingItem()
{
    bool mmsAutoRetr = getMsgSettings().getMmsAutoRetr();
    m_pAutoRetRoamingItem->disabled(!mmsAutoRetr);

    bool check = mmsAutoRetr ? getMsgSettings().getMmsAutoRetrRoaming() : getMmsAutoRetrRoamingBackup();
    m_pAutoRetRoamingItem->setCheckButtonState(check);
}

void Settings::alertsItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setAlerts(item.getCheckButtonState());
    updateAlertsItem();
    updateSoundItem();
    updateVibrationItem();
}

void Settings::soundItemHandler(SettingsListItem &item)
{
    MSG_LOG("soundItemHandler activated!");
    SettingsSound picker(getMsgSettings());
    picker.launchSoundPicker();
}

void Settings::vibrationItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setVibration(item.getCheckButtonState());
    updateVibrationItem();
}

void Settings::delivReportSmsItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setSmsDelivReport(item.getCheckButtonState());
    updateDelivReportSmsItem();
}

void Settings::msgOnSimItemHandler(SettingsListItem &item)
{
    MSG_LOG("");

    MsgOnSimCard *frame = new MsgOnSimCard(getParent());
    getParent().push(*frame);
}

void Settings::delivReportMmsItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setMmsDelivReport(item.getCheckButtonState());
    updateDelivReportMmsItem();
}

void Settings::readReportItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setMmsReadReport(item.getCheckButtonState());
    updateDelivReportMmsItem();
}

void Settings::autoRetItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setMmsAutoRetr(item.getCheckButtonState());
    bool mmsAutoRetr = getMsgSettings().getMmsAutoRetr();
    if(mmsAutoRetr)
    {
        // Restore:
        if(isExistingMmsAutoRetrRoamingBackup())
            getMsgSettings().setMmsAutoRetrRoaming(getMmsAutoRetrRoamingBackup());
    }
    else
    {
        // Save:
        setMmsAutoRetrRoamingBackup(getMsgSettings().getMmsAutoRetrRoaming());
        getMsgSettings().setMmsAutoRetrRoaming(false);
    }

    updateAutoRetItem();
    updateAutoRetRoamingItem();
}

void Settings::autoRetRoamingItemHandler(SettingsListItem &item)
{
    MSG_LOG("");
    getMsgSettings().setMmsAutoRetrRoaming(item.getCheckButtonState());
    updateAutoRetRoamingItem();
}

void Settings::onAttached(ViewItem &item)
{
    FrameController::onAttached(item);
    getNaviBar().setColor(NaviBar::NaviWhiteColorId);
    getNaviBar().setTitle(msgt("IDS_MSG_HEADER_SETTINGS"));
    getNaviBar().showButton(NaviPrevButtonId, true);
    setContent(*m_pList);
}

void Settings::callItemHandler(SettingsListItem &item)
{
    SettingsListItem::MethodHandler m = item.getHandler();
    if(m)
        (this->*m)(item);
}

void Settings::onListItemSelected(ListItem &listItem)
{
    MSG_LOG("Item: ", &listItem);
    listItem.setSelected(false);
    SettingsListItem *settingItem = dynamic_cast<SettingsListItem*>(&listItem);
    if(settingItem)
    {
        settingItem->changeCheckButtonState();
        callItemHandler(*settingItem);
    }
}

void Settings::onCheckButtonChanged(SettingsListViewItem &item)
{
    MSG_LOG("Item: ", &item);
    SettingsListItem *settingItem = dynamic_cast<SettingsListItem*>(&item);
    if(settingItem)
        callItemHandler(*settingItem);
}

void Settings::onNotiSoundChanged(MsgSettings &msgSetting)
{
    updateSoundItem();
    m_pSoundItem->update();
}

void Settings::onButtonClicked(NaviFrameItem &item, NaviButtonId buttonId)
{
    if(buttonId == NaviPrevButtonId)
    {
        getParent().pop();
    }
}
