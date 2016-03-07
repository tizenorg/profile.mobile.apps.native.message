/*
 * Copyright (c) 2009-2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "PopupBodyAttachmentListItem.h"
#include "BodyAttachmentViewItem.h"

using namespace Msg;

PopupBodyAttachmentListItem::PopupBodyAttachmentListItem(PopupList &parent,
        const std::string &text,
        PageViewItem &pageItem,
        PopupListItemPressedCb cb,
        void *userData)
    : PopupTextListItem(parent, text, cb, userData)
    , m_PageItem(pageItem)
{
}

PopupBodyAttachmentListItem::~PopupBodyAttachmentListItem()
{
}

PageViewItem &PopupBodyAttachmentListItem::getPageItem()
{
    return m_PageItem;
}

BodyAttachmentListItem::BodyAttachmentListItem(PopupList &parent,
        const std::string &text,
        BodyAttachmentViewItem &bodyItem,
        PopupListItemPressedCb cb,
        void *userData)
    : PopupTextListItem(parent, text, cb, userData)
    , m_BodyItem(bodyItem)
{
}

BodyAttachmentListItem::~BodyAttachmentListItem()
{
}

BodyAttachmentViewItem &BodyAttachmentListItem::getBodyItem()
{
    return m_BodyItem;
}
