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

#include "BubbleUnknownFileViewItem.h"
#include "Resource.h"

using namespace Msg;

BubbleUnknownFileViewItem::BubbleUnknownFileViewItem(BubbleEntity &entity, Evas_Object *parent)
    : BubbleIconTextLayoutItem(entity, parent, Layout1Icon1Text)
{
    attachGestureTapLayer(getEo(), getEo());
    setIcon(createIcon(getEo(), ATTACH_UNKNOWN_ICON));
}

BubbleUnknownFileViewItem::~BubbleUnknownFileViewItem()
{
}

BubbleUnknownFileEntity::BubbleUnknownFileEntity(const std::string &filePath, const std::string &fileName)
    : BubbleEntity(UnknownFileItem)
    , m_FilePath(filePath)
    , m_FileName(fileName)
{
}

BubbleUnknownFileEntity::~BubbleUnknownFileEntity()
{

}

BubbleUnknownFileViewItem *BubbleUnknownFileEntity::createView(Evas_Object *parent)
{
    auto *item = new BubbleUnknownFileViewItem(*this, parent);
    item->setMainText(m_FileName);
    return item;
}

const std::string &BubbleUnknownFileEntity::getFilePath() const
{
    return m_FilePath;
}


