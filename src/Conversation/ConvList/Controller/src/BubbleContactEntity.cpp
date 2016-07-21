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

#include "BubbleContactEntity.h"
#include "ContactManager.h"
#include "Contact.h"
#include "Resource.h"
#include "FileUtils.h"

using namespace Msg;


BubbleContactEntity::BubbleContactEntity(App &app, const MsgConvMedia &media)
    : BubbleEntity(ContactItem)
    , m_ThumbMaker(app.getThumbnailMaker())
    , m_FilePath(media.getPath())
{
    auto list = app.getContactManager().parseVcard(m_FilePath);
    if(!list.empty())
    {
        if(list.size() == 1)
        {
            m_Contact = list.front();
            if(m_Contact)
            {
                m_Name = m_Contact->getDispName();
                m_Address = m_Contact->getAddress();
                m_ThumbPath = m_Contact->getThumb();
            }
        }
    }
    if(!m_Contact)
        m_Name = getFileName(media);
}

BubbleContactEntity::~BubbleContactEntity()
{
}

BubbleContactViewItem *BubbleContactEntity::createView(Evas_Object *parent)
{
    BubbleContactViewItem::LayoutType type = m_Address.empty() ? BubbleContactViewItem::Layout1Icon1Text :
                                                                 BubbleContactViewItem::Layout1Icon2Text;
    auto *item = new BubbleContactViewItem(*this, parent, type);
    item->setMainText(m_Name);

    Evas_Object *thumb = nullptr;
    if(m_ThumbPath.empty())
    {
        ThumbId id = m_ThumbMaker.getThumbId(ThumbnailMaker::SingleThumb);
        thumb = m_ThumbMaker.getThumb(*item, id, BubbleContactViewItem::iconSize);
    }
    else
    {
        thumb = m_ThumbMaker.getThumb(*item, m_ThumbPath, BubbleContactViewItem::iconSize);
    }

    if(thumb)
        item->setIcon(thumb);
    if(!m_Address.empty())
        item->setSubText(m_Address);
    return item;
}

std::string BubbleContactEntity::getFilePath() const
{
    return m_FilePath;
}
