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

#include "MsgMedia.h"
#include "FileUtils.h"
#include "MsgMedia.h"

#include <unordered_map>
#include <string>
#include <string.h>
#include <algorithm>

namespace Msg
{
    struct MediaTypeData
    {
        MediaTypeData(const char *aMime, MsgMedia::Type aType)
            : mime(aMime)
            , type(aType)
        {
        }
        std::string mime;
        MsgMedia::Type type;
    };

    const std::unordered_multimap<std::string, MediaTypeData> mediaTypeByExtMap =
    {
        // Sound:
        {"aac", {"audio/aac", MsgMedia::AudioType}},
        {"amr", {"audio/amr", MsgMedia::AudioType}},
        {"amr", {"audio/x-amr", MsgMedia::AudioType}},
        {"amr", {"audio/amr-wb", MsgMedia::AudioType}},
        {"imy", {"audio/imelody", MsgMedia::AudioType}},
        {"imy", {"audio/imy", MsgMedia::AudioType}},
        {"imy", {"audio/iMelody", MsgMedia::AudioType}},
        {"imy", {"audio/melody", MsgMedia::AudioType}},
        {"imy", {"audio/x-iMelody", MsgMedia::AudioType}},
        {"imy", {"text/iMelody", MsgMedia::AudioType}},
        {"imy", {"text/x-iMelody", MsgMedia::AudioType}},
        {"imy", {"text/x-imelody", MsgMedia::AudioType}},
        {"mid", {"audio/mid", MsgMedia::AudioType}},
        {"mid", {"audio/midi", MsgMedia::AudioType}},
        {"mid", {"audio/x-mid", MsgMedia::AudioType}},
        {"mid", {"audio/x-midi", MsgMedia::AudioType}},
        {"midi", {"audio/mid", MsgMedia::AudioType}},
        {"mmf", {"application/vnd.smaf", MsgMedia::AudioType}},
        {"mmf", {"application/x-smaf", MsgMedia::AudioType}},
        {"mmf", {"audio/mmf", MsgMedia::AudioType}},
        {"mmf", {"audio/smaf", MsgMedia::AudioType}},
        {"mmf", {"audio/x-mmf", MsgMedia::AudioType}},
        {"mmf", {"audio/x-smaf", MsgMedia::AudioType}},
        {"mp3", {"audio/mpeg", MsgMedia::AudioType}},
        {"mp3", {"audio/mp3", MsgMedia::AudioType}},
        {"mp3", {"audio/mpg3", MsgMedia::AudioType}},
        {"mp3", {"audio/mpeg3", MsgMedia::AudioType}},
        {"mp3", {"audio/mpg", MsgMedia::AudioType}},
        {"mp3", {"audio/x-mp3", MsgMedia::AudioType}},
        {"mp3", {"audio/x-mpeg", MsgMedia::AudioType}},
        {"mp3", {"audio/x-mpeg3", MsgMedia::AudioType}},
        {"mp3", {"audio/x-mpegaudio", MsgMedia::AudioType}},
        {"mp3", {"audio/x-mpg", MsgMedia::AudioType}},
        {"mpg", {"audio/x-mpeg", MsgMedia::AudioType}},
        {"m4a", {"audio/m4a", MsgMedia::AudioType}},
        {"ra", {"audio/x-pn-realaudio", MsgMedia::AudioType}},
        {"rm", {"audio/vnd.rn-realaudio", MsgMedia::AudioType}},
        {"rm", {"audio/x-pn-multirate-realaudio", MsgMedia::AudioType}},
        {"rm", {"audio/x-pn-multirate-realaudio-live", MsgMedia::AudioType}},
        {"spm", {"audio/sp-midi", MsgMedia::AudioType}},
        {"wav", {"audio/wav", MsgMedia::AudioType}},
        {"wav", {"audio/wave", MsgMedia::AudioType}},
        {"wav", {"audio/x-wav", MsgMedia::AudioType}},
        {"wav", {"audio/x-wave", MsgMedia::AudioType}},
        {"wma", {"audio/wma", MsgMedia::AudioType}},
        {"wma", {"audio/x-ms-wma", MsgMedia::AudioType}},
        {"xmf", {"audio/mobile-xmf", MsgMedia::AudioType}},
        {"xmf", {"audio/xmf", MsgMedia::AudioType}},
        {"xmf", {"audio/x-xmf", MsgMedia::AudioType}},
        {"3gp", {"audio/3gpp", MsgMedia::AudioType}},
        {"mp4", {"audio/mp4", MsgMedia::AudioType}},
        {"mp4", {"audio/MP4A-LATM", MsgMedia::AudioType}},
        {"mp4", {"audio/mpeg4", MsgMedia::AudioType}},

        // Image:
        {"bmp", {"image/bmp", MsgMedia::ImageType}},
        {"bmp", {"image/x-bmp", MsgMedia::ImageType}},
        {"gif", {"image/gif", MsgMedia::ImageType}},
        {"GIF", {"image/gif", MsgMedia::ImageType}},
        {"gif", {"image/vnd.tmo.my5-gif", MsgMedia::ImageType}},
        {"jpg", {"image/jpg", MsgMedia::ImageType}},
        {"jpg", {"image/jpeg", MsgMedia::ImageType}},
        {"jpg", {"image/vnd.tmo.my5-jpg", MsgMedia::ImageType}},
        {"jpeg", {"image/jpeg", MsgMedia::ImageType}},
        {"JPG", {"image/jpeg", MsgMedia::ImageType}},
        {"jpg", {"image/pjpeg", MsgMedia::ImageType}},
        {"jpe", {"image/jpeg", MsgMedia::ImageType}},
        {"png", {"image/png", MsgMedia::ImageType}},
        {"wbmp", {"image/vnd.wap.wbmp", MsgMedia::ImageType}},
        {"wbmp", {"image/wbmp", MsgMedia::ImageType}},
        {"swf", {"application/x-shockwave-flash", MsgMedia::UnknownType}},
        {"tif", {"image/tiff", MsgMedia::ImageType}},
        {"tif", {"image/tif", MsgMedia::ImageType}},
        {"pmd", {"audio/pmd", MsgMedia::ImageType}},

        // Video:
        {"3gp", {"video/3gpp", MsgMedia::VideoType}},
        {"3gp", {"video/3gp", MsgMedia::VideoType}},
        {"3gp", {"video/h263", MsgMedia::VideoType}},
        {"3gp", {"video/mp4v-es", MsgMedia::VideoType}},
        {"asf", {"audio/x-ms-asf", MsgMedia::VideoType}},
        {"asf", {"video/x-ms-asf", MsgMedia::VideoType}},
        {"asx", {"video/x-ms-asf", MsgMedia::VideoType}},
        {"avi", {"video/avi", MsgMedia::VideoType}},
        {"mp2", {"video/mpeg", MsgMedia::VideoType}},
        {"mp4", {"video/mp4", MsgMedia::VideoType}},
        {"mp4", {"video/mpeg4", MsgMedia::VideoType}},
        {"mp4", {"video/x-mp4", MsgMedia::VideoType}},
        {"mp4", {"video/x-pv-mp4", MsgMedia::VideoType}},
        {"mpg", {"video/mpeg", MsgMedia::VideoType}},
        {"mpeg", {"video/mpeg", MsgMedia::VideoType}},
        {"rm", {"video/x-pn-multirate-realvideo", MsgMedia::VideoType}},
        {"rm", {"video/vnd.rn-realmedia", MsgMedia::VideoType}},
        {"rm", {"video/vnd.rn-realvideo", MsgMedia::VideoType}},
        {"rv", {"video/x-pn-realvideo", MsgMedia::VideoType}},
        {"sdp", {"application/sdp", MsgMedia::VideoType}},
        {"sdp", {"video/sdp", MsgMedia::VideoType}},
        {"wmv", {"video/x-ms-wmv", MsgMedia::VideoType}},

        // Text:
        {"txt", {"text/plain", MsgMedia::TextType}},
        {"text", {"text/plain", MsgMedia::UnknownType}},
        {"html", {"text/html", MsgMedia::UnknownType}},
        {"htm", {"text/html", MsgMedia::UnknownType}},
        {"wml", {"text/vnd.wap.wml", MsgMedia::UnknownType}},
        {"xml", {"text/xml", MsgMedia::UnknownType}},

        // vObject:
        {"vbm", {"text/x-vbookmark", MsgMedia::UnknownType}},
        {"vcf", {"text/x-vcard", MsgMedia::UnknownType}},
        {"vcs", {"text/x-vCalendar", MsgMedia::UnknownType}},
        {"vnt", {"text/x-vnote", MsgMedia::UnknownType}},

        // Doc:
        {"doc", {"application/msword", MsgMedia::UnknownType}},
        {"doc", {"applcation/vnd.ms-word", MsgMedia::UnknownType}},
        {"xls", {"application/vnd.ms-excel", MsgMedia::UnknownType}},
        {"xls", {"application/x-msexcel", MsgMedia::UnknownType}},
        {"ppt", {"application/vnd.ms-powerpoint", MsgMedia::UnknownType}},
        {"ppt", {"application/x-mspowerpoint", MsgMedia::UnknownType}},
        {"pdf", {"application/pdf", MsgMedia::UnknownType}},

        // Etc:
        {"rm ", {"application/vnd.rn-realmedia", MsgMedia::UnknownType}},
        {"smi", {"application/smil", MsgMedia::UnknownType}},
        {"smil", {"application/smil", MsgMedia::UnknownType}},
        {"xml", {"application/vnd.oma.drm.roap-pdu+xml", MsgMedia::UnknownType}},
        {"xml", {"application/vnd.oma.drm.roap-trigger+xml", MsgMedia::UnknownType}},
        {"xxx", {"application/octet-stream", MsgMedia::UnknownType}},
    };

    const std::unordered_multimap<std::string, MsgMedia::Type> mediaTypeByMimeMap =
    {
        // Sound:
        {"audio/aac", MsgMedia::AudioType},
        {"audio/amr", MsgMedia::AudioType},
        {"audio/x-amr", MsgMedia::AudioType},
        {"audio/amr-wb", MsgMedia::AudioType},
        {"audio/imelody", MsgMedia::AudioType},
        {"audio/imy", MsgMedia::AudioType},
        {"audio/iMelody", MsgMedia::AudioType},
        {"audio/melody", MsgMedia::AudioType},
        {"audio/x-iMelody", MsgMedia::AudioType},
        {"text/iMelody", MsgMedia::AudioType},
        {"text/x-iMelody", MsgMedia::AudioType},
        {"text/x-imelody", MsgMedia::AudioType},
        {"audio/mid", MsgMedia::AudioType},
        {"audio/midi", MsgMedia::AudioType},
        {"audio/x-mid", MsgMedia::AudioType},
        {"audio/x-midi", MsgMedia::AudioType},
        {"audio/mid", MsgMedia::AudioType},
        {"application/vnd.smaf", MsgMedia::AudioType},
        {"application/x-smaf", MsgMedia::AudioType},
        {"audio/mmf", MsgMedia::AudioType},
        {"audio/smaf", MsgMedia::AudioType},
        {"audio/x-mmf", MsgMedia::AudioType},
        {"audio/x-smaf", MsgMedia::AudioType},
        {"audio/mpeg", MsgMedia::AudioType},
        {"audio/mp3", MsgMedia::AudioType},
        {"audio/mpg3", MsgMedia::AudioType},
        {"audio/mpeg3", MsgMedia::AudioType},
        {"audio/mpg", MsgMedia::AudioType},
        {"audio/x-mp3", MsgMedia::AudioType},
        {"audio/x-mpeg", MsgMedia::AudioType},
        {"audio/x-mpeg3", MsgMedia::AudioType},
        {"audio/x-mpegaudio", MsgMedia::AudioType},
        {"audio/x-mpg", MsgMedia::AudioType},
        {"audio/x-mpeg", MsgMedia::AudioType},
        {"audio/m4a", MsgMedia::AudioType},
        {"audio/x-pn-realaudio", MsgMedia::AudioType},
        {"audio/vnd.rn-realaudio", MsgMedia::AudioType},
        {"audio/x-pn-multirate-realaudio", MsgMedia::AudioType},
        {"audio/x-pn-multirate-realaudio-live", MsgMedia::AudioType},
        {"audio/sp-midi", MsgMedia::AudioType},
        {"audio/wav", MsgMedia::AudioType},
        {"audio/wave", MsgMedia::AudioType},
        {"audio/x-wav", MsgMedia::AudioType},
        {"audio/x-wave", MsgMedia::AudioType},
        {"audio/wma", MsgMedia::AudioType},
        {"audio/x-ms-wma", MsgMedia::AudioType},
        {"audio/mobile-xmf", MsgMedia::AudioType},
        {"audio/xmf", MsgMedia::AudioType},
        {"audio/x-xmf", MsgMedia::AudioType},
        {"audio/3gpp", MsgMedia::AudioType},
        {"audio/mp4", MsgMedia::AudioType},
        {"audio/MP4A-LATM", MsgMedia::AudioType},
        {"audio/mpeg4", MsgMedia::AudioType},

        // Image:
        {"image/bmp", MsgMedia::ImageType},
        {"image/x-bmp", MsgMedia::ImageType},
        {"image/gif", MsgMedia::ImageType},
        {"image/gif", MsgMedia::ImageType},
        {"image/vnd.tmo.my5-gif", MsgMedia::ImageType},
        {"image/jpg", MsgMedia::ImageType},
        {"image/jpeg", MsgMedia::ImageType},
        {"image/vnd.tmo.my5-jpg", MsgMedia::ImageType},
        {"image/jpeg", MsgMedia::ImageType},
        {"image/jpeg", MsgMedia::ImageType},
        {"image/pjpeg", MsgMedia::ImageType},
        {"image/jpeg", MsgMedia::ImageType},
        {"image/png", MsgMedia::ImageType},
        {"image/vnd.wap.wbmp", MsgMedia::ImageType},
        {"image/wbmp", MsgMedia::ImageType},
        {"application/x-shockwave-flash", MsgMedia::UnknownType},
        {"image/tiff", MsgMedia::ImageType},
        {"image/tif", MsgMedia::ImageType},
        {"audio/pmd", MsgMedia::ImageType},

        // Video:
        {"video/3gpp", MsgMedia::VideoType},
        {"video/3gp", MsgMedia::VideoType},
        {"video/h263", MsgMedia::VideoType},
        {"video/mp4v-es", MsgMedia::VideoType},
        {"audio/x-ms-asf", MsgMedia::VideoType},
        {"video/x-ms-asf", MsgMedia::VideoType},
        {"video/x-ms-asf", MsgMedia::VideoType},
        {"video/avi", MsgMedia::VideoType},
        {"video/mpeg", MsgMedia::VideoType},
        {"video/mp4", MsgMedia::VideoType},
        {"video/mpeg4", MsgMedia::VideoType},
        {"video/x-mp4", MsgMedia::VideoType},
        {"video/x-pv-mp4", MsgMedia::VideoType},
        {"video/mpeg", MsgMedia::VideoType},
        {"video/mpeg", MsgMedia::VideoType},
        {"video/x-pn-multirate-realvideo", MsgMedia::VideoType},
        {"video/vnd.rn-realmedia", MsgMedia::VideoType},
        {"video/vnd.rn-realvideo", MsgMedia::VideoType},
        {"video/x-pn-realvideo", MsgMedia::VideoType},
        {"application/sdp", MsgMedia::VideoType},
        {"video/sdp", MsgMedia::VideoType},
        {"video/x-ms-wmv", MsgMedia::VideoType},

        // Text:
        {"text/plain", MsgMedia::TextType}
    };

    inline static const MediaTypeData getMsgMediaTypeByFileExt(const std::string &filePath)
    {
        std::string base;
        std::string name;
        std::string ext;

        FileUtils::splitPath(filePath, base, name, ext);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        auto it = mediaTypeByExtMap.find(ext);
        return it != mediaTypeByExtMap.end() ? it->second : MediaTypeData("", MsgMedia::UnknownType);
    }

    inline static MsgMedia::Type getMsgMediaTypeByMime(std::string mime)
    {
        std::transform(mime.begin(), mime.end(), mime.begin(), ::tolower);

        auto it = mediaTypeByMimeMap.find(mime);
        return it != mediaTypeByMimeMap.end() ? it->second : MsgMedia::UnknownType;
    }
}

#endif /* BodyMediaType_h_ */
