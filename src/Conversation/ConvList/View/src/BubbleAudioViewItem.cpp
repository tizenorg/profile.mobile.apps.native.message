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

#include "BubbleAudioViewItem.h"
#include "Resource.h"

using namespace Msg;

BubbleAudioViewItem::BubbleAudioViewItem(BubbleEntity &entity, Evas_Object *parent)
    : BubbleIconTextLayoutItem(entity, parent, Layout1Icon2Text)
{
    attachGestureTapLayer(getEo(), getEo());
}

BubbleAudioViewItem::~BubbleAudioViewItem()
{
}


BubbleAudioEntity::BubbleAudioEntity(const std::string &filePath, const std::string &fileName, const std::string &duration)
    : BubbleEntity(AudioItem)
    , m_FilePath(filePath)
    , m_FileName(fileName)
    , m_Duration(duration)
{

}

BubbleAudioEntity::~BubbleAudioEntity()
{

}

BubbleAudioViewItem *BubbleAudioEntity::createView(Evas_Object *parent)
{
    auto *item = new BubbleAudioViewItem(*this, parent);
    item->setIcon(BubbleAudioViewItem::createIcon(*item, ATTACH_MUSIC_ICON));
    item->setMainText(m_FileName);
    item->setSubText(m_Duration);
    return item;
}

const std::string &BubbleAudioEntity::getFilePath() const
{
    return m_FilePath;
}


