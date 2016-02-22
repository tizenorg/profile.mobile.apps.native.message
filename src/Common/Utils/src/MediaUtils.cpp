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

#include "MediaUtils.h"

#include <metadata_extractor.h>
#include <player.h>
#include <unistd.h>

using namespace Msg;

std::string MediaUtils::getMediaTitle(const std::string &path)
{
    char *alert_title = nullptr;
    int ret = METADATA_EXTRACTOR_ERROR_NONE;
    metadata_extractor_h metadata = NULL;
    if(access( path.c_str(), F_OK ) < 0)
        return path;

    ret = metadata_extractor_create(&metadata);
    if (ret != METADATA_EXTRACTOR_ERROR_NONE)
        return path;

    ret = metadata_extractor_set_path(metadata, path.c_str());
    if(ret != METADATA_EXTRACTOR_ERROR_NONE)
    {
        metadata_extractor_destroy(metadata);
        return path;
    }

    ret = metadata_extractor_get_metadata(metadata, METADATA_TITLE, &alert_title);
    if(ret != METADATA_EXTRACTOR_ERROR_NONE)
    {
        metadata_extractor_destroy(metadata);
        return path;
    }

    metadata_extractor_destroy(metadata);
    return alert_title ? alert_title : "";
}

int MediaUtils::getDuration(const std::string &uri)
{
    int msec = 0;
    if(!uri.empty())
    {
        player_h player {};
        player_create(&player);

        if(player_set_uri(player, uri.c_str()) == PLAYER_ERROR_NONE)
        {
            player_prepare(player);
            player_get_duration(player, &msec);
            player_unprepare(player);
        }
        player_destroy(player);
    }
    return msec;
}

