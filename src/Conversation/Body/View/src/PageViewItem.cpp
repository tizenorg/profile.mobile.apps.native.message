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

#include "PageViewItem.h"
#include "Resource.h"
#include "PageView.h"

using namespace Msg;

PageViewItem::PageViewItem(PageView &parent)
    : m_Parent(parent)
{

}

PageViewItem::~PageViewItem()
{

}

void PageViewItem::destroy()
{
    m_Parent.removeItem(*this);
}

const std::string &PageViewItem::getEdjPath() const
{
    static std::string path = ResourceUtils::getResourcePath(MSG_BODY_EDJ_PATH);
    return path;
}

const PageView &PageViewItem::getParentPage() const
{
    return m_Parent;
}

PageView &PageViewItem::getParentPage()
{
    return m_Parent;
}

