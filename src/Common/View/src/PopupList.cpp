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

#include "PopupList.h"
#include "PopupAttachmentListItem.h"
#include <string.h>

using namespace Msg;

PopupList::PopupList(Evas_Object *parent)
    : Popup(parent)
    , m_pList(nullptr)
{
    create();
}

PopupList::PopupList(PopupManager &parent)
    : Popup(parent)
    , m_pList(nullptr)
{
    create();
}

PopupList::~PopupList()
{
}

void PopupList::appendItem(PopupListItem &item)
{
    m_pList->appendItem(item);
}

void PopupList::appendItem(const std::string &text, PopupListItemPressedCb cb, void *userData)
{
    m_pList->appendItem(*new PopupTextListItem(*this, text, cb, userData));
}

void PopupList::appendItem(const std::string &text, const std::string &path, PopupListItemPressedCb cb, void *userData)
{
    m_pList->appendItem(*new PopupAttachmentListItem(*this, text, path, cb, userData));
}

void PopupList::create()
{
    Evas_Object *list = createList(getHostEvasObject());
    Popup::setContent(list);
}

Evas_Object *PopupList::createList(Evas_Object *parent)
{
    m_pList = new ListView(parent);
    m_pList->setListener(this);
    m_pList->show();
    m_pList->setMultiSelection(false);
    m_pList->setMode(ELM_LIST_EXPAND);
    return *m_pList;
}

ListView &PopupList::getListView()
{
    return *m_pList;
}

void PopupList::onListItemSelected(ListItem &listItem)
{
    PopupListItem &it = static_cast<PopupListItem&>(listItem);
    it.fireCallback();
}
