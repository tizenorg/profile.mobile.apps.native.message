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

#include "BubbleDownloadButtonViewItem.h"
#include "LangUtils.h"

using namespace Msg;

BubbleDownloadButtonViewItem::BubbleDownloadButtonViewItem(BubbleEntity &entity, Evas_Object *parent)
    : BubbleViewItem(entity)
{
    Evas_Object *button = elm_button_add(parent);
    setEo(button);
    expand();
    setText(msgt("IDS_MSG_BUTTON_DOWNLOAD_ABB3"));
    evas_object_smart_callback_add(button, "clicked", SMART_CALLBACK(BubbleDownloadButtonViewItem, onPressed), this);
    evas_object_smart_calculate(button);
    evas_object_show(button);
}

BubbleDownloadButtonViewItem::~BubbleDownloadButtonViewItem()
{

}

void BubbleDownloadButtonViewItem::onPressed(Evas_Object *obj, void *eventInfo)
{
    emitActionEvent();
}

BubbleDownloadButtonEntity::BubbleDownloadButtonEntity()
    : BubbleEntity(DownloadButtonItem)
{
}

BubbleDownloadButtonEntity::~BubbleDownloadButtonEntity()
{
}

BubbleDownloadButtonViewItem *BubbleDownloadButtonEntity::createView(Evas_Object *parent)
{
    auto *item = new BubbleDownloadButtonViewItem(*this, parent);
    return item;
}

const std::string &BubbleDownloadButtonEntity::getFilePath() const
{
    static std::string empty;
    return empty;
}
