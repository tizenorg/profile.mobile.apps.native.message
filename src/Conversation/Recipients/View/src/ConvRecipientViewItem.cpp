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


#include "ConvRecipientViewItem.h"

using namespace Msg;

ConvRecipientViewItem::ConvRecipientViewItem()
    : ViewItem()
    , m_pOwner(nullptr)
{
}

ConvRecipientViewItem::~ConvRecipientViewItem()
{

}

void ConvRecipientViewItem::onViewDestroyed()
{
}

void ConvRecipientViewItem::setDisplayName(const std::string &displayName)
{
    m_DisplayName = displayName;
}

const std::string &ConvRecipientViewItem::getDisplayName() const
{
    return m_DisplayName;
}
