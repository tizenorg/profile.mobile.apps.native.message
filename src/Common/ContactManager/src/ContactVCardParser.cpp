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

std::list<ContactVCard> ContactManager::parseVcard(const std::string &filePath)
{
    std::list<ContactVCard> list;

    if(filePath.empty())
        return {};

    std::string vcardStream = FileUtils::readTextFile(filePath);
    if(vcardStream.empty())
        return {};

  /*  contacts_list_h list = {};
    contacts_vcard_parse_to_contacts(vcardStream.c_str(), &list);

    contacts_record_h record = {};
    while(contacts_list_get_current_record_p(contacts_list, &record) == 0)
    {

    }

    contacts_list_destroy(list, false);*/

    return list;

}
