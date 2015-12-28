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

#ifndef BodyMediaType_h_
#define BodyMediaType_h_

#include "PageViewItem.h"
#include "FileUtils.h"

#include <unordered_map>
#include <string>
#include <string.h>
#include <algorithm>

namespace Msg
{
    struct MediaTypeData
    {
        MediaTypeData(const char *aMime, PageViewItem::Type aType)
            : mime(aMime)
            , type(aType)
        {
        }
        const char *mime;
        PageViewItem::Type type;
    };

    const std::unordered_map<std::string, MediaTypeData> mediaTypeMap =
    {
        // Sound:
        {"aac", {"audio/aac", PageViewItem::SoundType}},
        {"amr", {"audio/amr", PageViewItem::SoundType}},
        {"amr", {"audio/x-amr", PageViewItem::SoundType}},
        {"amr", {"audio/amr-wb", PageViewItem::SoundType}},
        {"imy", {"audio/imelody", PageViewItem::SoundType}},
        {"imy", {"audio/imy", PageViewItem::SoundType}},
        {"imy", {"audio/iMelody", PageViewItem::SoundType}},
        {"imy", {"audio/melody", PageViewItem::SoundType}},
        {"imy", {"audio/x-iMelody", PageViewItem::SoundType}},
        {"imy", {"text/iMelody", PageViewItem::SoundType}},
        {"imy", {"text/x-iMelody", PageViewItem::SoundType}},
        {"imy", {"text/x-imelody", PageViewItem::SoundType}},
        {"mid", {"audio/mid", PageViewItem::SoundType}},
        {"mid", {"audio/midi", PageViewItem::SoundType}},
        {"mid", {"audio/x-mid", PageViewItem::SoundType}},
        {"mid", {"audio/x-midi", PageViewItem::SoundType}},
        {"midi", {"audio/mid", PageViewItem::SoundType}},
        {"mmf", {"application/vnd.smaf", PageViewItem::SoundType}},
        {"mmf", {"application/x-smaf", PageViewItem::SoundType}},
        {"mmf", {"audio/mmf", PageViewItem::SoundType}},
        {"mmf", {"audio/smaf", PageViewItem::SoundType}},
        {"mmf", {"audio/x-mmf", PageViewItem::SoundType}},
        {"mmf", {"audio/x-smaf", PageViewItem::SoundType}},
        {"mp3", {"audio/mpeg", PageViewItem::SoundType}},
        {"mp3", {"audio/mp3", PageViewItem::SoundType}},
        {"mp3", {"audio/mpg3", PageViewItem::SoundType}},
        {"mp3", {"audio/mpeg3", PageViewItem::SoundType}},
        {"mp3", {"audio/mpg", PageViewItem::SoundType}},
        {"mp3", {"audio/x-mp3", PageViewItem::SoundType}},
        {"mp3", {"audio/x-mpeg", PageViewItem::SoundType}},
        {"mp3", {"audio/x-mpeg3", PageViewItem::SoundType}},
        {"mp3", {"audio/x-mpegaudio", PageViewItem::SoundType}},
        {"mp3", {"audio/x-mpg", PageViewItem::SoundType}},
        {"mpg", {"audio/x-mpeg", PageViewItem::SoundType}},
        {"m4a", {"audio/m4a", PageViewItem::SoundType}},
        {"ra", {"audio/x-pn-realaudio", PageViewItem::SoundType}},
        {"rm", {"audio/vnd.rn-realaudio", PageViewItem::SoundType}},
        {"rm", {"audio/x-pn-multirate-realaudio", PageViewItem::SoundType}},
        {"rm", {"audio/x-pn-multirate-realaudio-live", PageViewItem::SoundType}},
        {"spm", {"audio/sp-midi", PageViewItem::SoundType}},
        {"wav", {"audio/wav", PageViewItem::SoundType}},
        {"wav", {"audio/wave", PageViewItem::SoundType}},
        {"wav", {"audio/x-wav", PageViewItem::SoundType}},
        {"wav", {"audio/x-wave", PageViewItem::SoundType}},
        {"wma", {"audio/wma", PageViewItem::SoundType}},
        {"wma", {"audio/x-ms-wma", PageViewItem::SoundType}},
        {"xmf", {"audio/mobile-xmf", PageViewItem::SoundType}},
        {"xmf", {"audio/xmf", PageViewItem::SoundType}},
        {"xmf", {"audio/x-xmf", PageViewItem::SoundType}},
        {"3gp", {"audio/3gpp", PageViewItem::SoundType}},
        {"mp4", {"audio/mp4", PageViewItem::SoundType}},
        {"mp4", {"audio/MP4A-LATM", PageViewItem::SoundType}},
        {"mp4", {"audio/mpeg4", PageViewItem::SoundType}},

        // Image:
        {"bmp", {"image/bmp", PageViewItem::ImageType}},
        {"bmp", {"image/x-bmp", PageViewItem::ImageType}},
        {"gif", {"image/gif", PageViewItem::ImageType}},
        {"GIF", {"image/gif", PageViewItem::ImageType}},
        {"gif", {"image/vnd.tmo.my5-gif", PageViewItem::ImageType}},
        {"jpg", {"image/jpg", PageViewItem::ImageType}},
        {"jpg", {"image/jpeg", PageViewItem::ImageType}},
        {"jpg", {"image/vnd.tmo.my5-jpg", PageViewItem::ImageType}},
        {"jpeg", {"image/jpeg", PageViewItem::ImageType}},
        {"JPG", {"image/jpeg", PageViewItem::ImageType}},
        {"jpg", {"image/pjpeg", PageViewItem::ImageType}},
        {"jpe", {"image/jpeg", PageViewItem::ImageType}},
        {"png", {"image/png", PageViewItem::ImageType}},
        {"wbmp", {"image/vnd.wap.wbmp", PageViewItem::ImageType}},
        {"wbmp", {"image/wbmp", PageViewItem::ImageType}},
        {"swf", {"application/x-shockwave-flash", PageViewItem::UnknownType}},
        {"tif", {"image/tiff", PageViewItem::ImageType}},
        {"tif", {"image/tif", PageViewItem::ImageType}},
        {"pmd", {"audio/pmd", PageViewItem::ImageType}},

        // Video:
        {"3gp", {"video/3gpp", PageViewItem::VideoType}},
        {"3gp", {"video/3gp", PageViewItem::VideoType}},
        {"3gp", {"video/h263", PageViewItem::VideoType}},
        {"3gp", {"video/mp4v-es", PageViewItem::VideoType}},
        {"asf", {"audio/x-ms-asf", PageViewItem::VideoType}},
        {"asf", {"video/x-ms-asf", PageViewItem::VideoType}},
        {"asx", {"video/x-ms-asf", PageViewItem::VideoType}},
        {"avi", {"video/avi", PageViewItem::VideoType}},
        {"mp2", {"video/mpeg", PageViewItem::VideoType}},
        {"mp4", {"video/mp4", PageViewItem::VideoType}},
        {"mp4", {"video/mpeg4", PageViewItem::VideoType}},
        {"mp4", {"video/x-mp4", PageViewItem::VideoType}},
        {"mp4", {"video/x-pv-mp4", PageViewItem::VideoType}},
        {"mpg", {"video/mpeg", PageViewItem::VideoType}},
        {"mpeg", {"video/mpeg", PageViewItem::VideoType}},
        {"rm", {"video/x-pn-multirate-realvideo", PageViewItem::VideoType}},
        {"rm", {"video/vnd.rn-realmedia", PageViewItem::VideoType}},
        {"rm", {"video/vnd.rn-realvideo", PageViewItem::VideoType}},
        {"rv", {"video/x-pn-realvideo", PageViewItem::VideoType}},
        {"sdp", {"application/sdp", PageViewItem::VideoType}},
        {"sdp", {"video/sdp", PageViewItem::VideoType}},
        {"wmv", {"video/x-ms-wmv", PageViewItem::VideoType}},

        // Text:
        {"txt", {"text/plain", PageViewItem::TextType}},
        {"text", {"text/plain", PageViewItem::TextType}},
        {"html", {"text/html", PageViewItem::TextType}},
        {"htm", {"text/html", PageViewItem::TextType}},
        {"wml", {"text/vnd.wap.wml", PageViewItem::TextType}},
        {"xml", {"text/xml", PageViewItem::TextType}},

        // vObject:
        {"vbm", {"text/x-vbookmark", PageViewItem::UnknownType}},
        {"vcf", {"text/x-vcard", PageViewItem::UnknownType}},
        {"vcs", {"text/x-vCalendar", PageViewItem::UnknownType}},
        {"vnt", {"text/x-vnote", PageViewItem::UnknownType}},

        // Doc:
        {"doc", {"application/msword", PageViewItem::UnknownType}},
        {"doc", {"applcation/vnd.ms-word", PageViewItem::UnknownType}},
        {"xls", {"application/vnd.ms-excel", PageViewItem::UnknownType}},
        {"xls", {"application/x-msexcel", PageViewItem::UnknownType}},
        {"ppt", {"application/vnd.ms-powerpoint", PageViewItem::UnknownType}},
        {"ppt", {"application/x-mspowerpoint", PageViewItem::UnknownType}},
        {"pdf", {"application/pdf", PageViewItem::UnknownType}},

        // Etc:
        {"rm ", {"application/vnd.rn-realmedia", PageViewItem::UnknownType}},
        {"smi", {"application/smil", PageViewItem::UnknownType}},
        {"smil", {"application/smil", PageViewItem::UnknownType}},
        {"xml", {"application/vnd.oma.drm.roap-pdu+xml", PageViewItem::UnknownType}},
        {"xml", {"application/vnd.oma.drm.roap-trigger+xml", PageViewItem::UnknownType}},
        {"xxx", {"application/octet-stream", PageViewItem::UnknownType}},
    };

    inline static const MediaTypeData getMediaType(const std::string &filePath)
    {
        std::string base;
        std::string name;
        std::string ext;

        FileUtils::splitPath(filePath, base, name, ext);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        auto it = mediaTypeMap.find(ext);
        return it != mediaTypeMap.end() ? it->second : MediaTypeData(nullptr, PageViewItem::UnknownType);
    }
}

#endif /* BodyMediaType_h_ */
