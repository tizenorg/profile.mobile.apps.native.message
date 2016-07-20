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

#include "BubbleContactViewItem.h"
#include "Resource.h"

using namespace Msg;

BubbleContactViewItem::BubbleContactViewItem(BubbleEntity &entity, Evas_Object *parent, LayoutType type)
    : BubbleIconTextLayoutItem(entity, parent, type)
{
    attachGestureTapLayer(getEo(), getEo());
}

BubbleContactViewItem::~BubbleContactViewItem()
{
}

BubbleContactEntity::BubbleContactEntity(ThumbnailMaker &thumbMaker, const std::string &filePath, const std::string &fileName)
    : BubbleContactEntity(thumbMaker, filePath, fileName, {})
{

}

BubbleContactEntity::BubbleContactEntity(ThumbnailMaker &thumbMaker, const std::string &filePath, const std::string &dispName, const std::string &address)
    : BubbleEntity(ContactItem)
    , m_FilePath(filePath)
    , m_Name(dispName)
    , m_Address(address)
    , m_ThumbId(ThumbnailMaker::SingleThumb)
    , m_ThumbMaker(thumbMaker)
{
}

BubbleContactEntity::~BubbleContactEntity()
{
}

BubbleContactViewItem *BubbleContactEntity::createView(Evas_Object *parent)
{
    BubbleContactViewItem::LayoutType type = m_Address.empty() ? BubbleContactViewItem::Layout1Icon1Text : BubbleContactViewItem::Layout1Icon2Text;
    auto *item = new BubbleContactViewItem(*this, parent, type);
    item->setIcon(m_ThumbMaker.getThumbById(*item, m_ThumbId, BubbleContactViewItem::iconSize));
    item->setMainText(m_Name);
    if(!m_Address.empty())
        item->setSubText(m_Address);
    return item;
}

const std::string &BubbleContactEntity::getFilePath() const
{
    return m_FilePath;
}

void BubbleContactEntity::setThumbId(ThumbId id)
{
    m_ThumbId = id;
}
