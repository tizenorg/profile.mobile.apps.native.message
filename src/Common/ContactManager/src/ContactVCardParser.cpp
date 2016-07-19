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

#include "ContactManager.h"
#include "Logger.h"
#include "FileUtils.h"

#include <sstream>
#include <contacts_vcard.h>
#include <contacts.h>
#include <contacts_views.h>

using namespace Msg;

std::shared_ptr<ContactList<Contact>> ContactManager::parseVcard(const std::string &filePath)
{
    std::shared_ptr<ContactList<Contact>> res;

    if(filePath.empty())
        return res;

    std::string vcardStream = FileUtils::readTextFile(filePath);
    if(vcardStream.empty())
        return res;

    contacts_list_h list = {};
    int parseRes = contacts_vcard_parse_to_contacts(vcardStream.c_str(), &list);
    if(list)
        res = std::make_shared<ContactList<Contact>>(list);

    MSG_LOG("Parse result: ", parseRes);

    return res;
}
