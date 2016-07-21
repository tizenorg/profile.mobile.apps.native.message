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

std::list<ContactRef> ContactManager::parseVcard(const std::string &filePath)
{
    std::list<ContactRef> res;

    if(filePath.empty())
        return res;

    int parseRes = contacts_vcard_parse_to_contact_foreach
    (
        filePath.c_str(),
        [](contacts_record_h record, void *user_data)->bool
        {
            contacts_record_h recordClone = {};
            contacts_record_clone(record, &recordClone);
            auto *list = (std::list<ContactRef>*)user_data;
            auto recRef = std::make_shared<Contact>(true, recordClone);
            list->emplace_back(recRef);
            return true;
        },
        &res
    );

    MSG_LOG("Parse result: ", parseRes);
    return res;
}
