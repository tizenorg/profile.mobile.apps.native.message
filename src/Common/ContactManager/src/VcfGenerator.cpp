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
#include "FileUtils.h"
#include <sstream>
#include "Logger.h"
#include <contacts_vcard.h>
#include <contacts.h>
#include <contacts_views.h>

using namespace Msg;

std::string ContactManager::vcardCreateFromId(const int personId, bool myProfile)
{
    contacts_record_h record = nullptr;
    std::string vcardContent;

    char *displayName = nullptr;
    if(contacts_db_get_record((myProfile ? _contacts_my_profile._uri : _contacts_person._uri), personId, &record) != CONTACTS_ERROR_NONE)
    {
        MSG_LOG_ERROR("contacts_db_get_record() failed");
        record = nullptr;
        return std::string();
    }
    vcardContent = createContactContent(record, myProfile);
    MSG_LOG("vcardContent = ", vcardContent);
    if(record)
        contacts_record_destroy(record, true);

    return vcardContent;
}

std::string ContactManager::vcardCreateFromIdList(const std::list<int> &idList)
{
    std::ostringstream vcardContent;

    for(auto it : idList)
    {
        if(!vcardContent.str().empty())
            vcardContent << "/n";

        vcardContent << createContentForContactList(it);
    }
    MSG_LOG("vcardContent = ", vcardContent.str());
    return vcardContent.str();
}

std::string ContactManager::createContactContent(contacts_record_h record, bool myProfile)
{
    char *vcardBuff = nullptr;

    myProfile ? contacts_vcard_make_from_my_profile(record, &vcardBuff) : contacts_vcard_make_from_person(record, &vcardBuff);

    if(!vcardBuff)
    {
        MSG_LOG_ERROR("vcardBuff is NULL");
        return std::string();
    }

    std::string result(vcardBuff);
    free(vcardBuff);
    return result;
}

std::string ContactManager::createContentForContactList(int personId)
{
    contacts_record_h record = nullptr;
    if(contacts_db_get_record(_contacts_person._uri, personId, &record) != CONTACTS_ERROR_NONE)
    {
        MSG_LOG_ERROR("contacts_db_get_record() failed!");
        record = nullptr;
        return std::string();
    }
    std::string result = createContactContent(record, false);

    if (record)
        contacts_record_destroy(record, true);

    return result;
}
