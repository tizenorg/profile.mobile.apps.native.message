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

#include "BubbleUnknownFileEntity.h"
#include "Resource.h"

#include "Logger.h"

using namespace Msg;

BubbleUnknownFileEntity::BubbleUnknownFileEntity(const MsgConvMedia &convMedia)
    : BubbleEntity(UnknownFileItem)
    , m_FilePath(convMedia.getPath())
    , m_FileName(getFileName(convMedia))
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

std::string BubbleUnknownFileEntity::getFilePath() const
{
    return m_FilePath;
}


