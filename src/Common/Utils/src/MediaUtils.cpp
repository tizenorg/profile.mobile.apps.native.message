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
#include "FileUtils.h"
#include "Logger.h"

#include <metadata_extractor.h>
#include <image_util.h>

using namespace Msg;

namespace
{
    class MetadataExtractor
    {
        public:
            MetadataExtractor(const std::string &path)
                : m_Metadata()
            {
                if(FileUtils::isExists(path))
                {
                    metadata_extractor_create(&m_Metadata);
                    if(metadata_extractor_set_path(m_Metadata, path.c_str()) != 0)
                    {
                        metadata_extractor_destroy(m_Metadata);
                        m_Metadata = nullptr;
                    }
                }
            }

            ~MetadataExtractor()
            {
                if(m_Metadata)
                    metadata_extractor_destroy(m_Metadata);
            }

            bool isValid() const
            {
                return m_Metadata != nullptr;
            }

            int getInt(metadata_extractor_attr_e attr) const
            {
                std::string str = getStr(attr);
                return str.empty() ? 0 : atoi(str.c_str());
            }

            std::string getStr(metadata_extractor_attr_e attr) const
            {
                std::string res;
                char *cStr = nullptr;
                metadata_extractor_get_metadata(m_Metadata, attr, &cStr);
                if(cStr)
                {
                    res = cStr;
                    free(cStr);
                }
                return res;
            }

            bool getFrame(void **frame, int *size) const
            {
                return metadata_extractor_get_frame(m_Metadata, frame, size) == 0;
            }

        private:
            metadata_extractor_h m_Metadata;
    };
}

std::string MediaUtils::getTitle(const std::string &path)
{
    std::string fileName = FileUtils::getFileName(path);

    MetadataExtractor extractor(path);
    if(!extractor.isValid())
        return fileName;

    std::string title = extractor.getStr(METADATA_TITLE);
    return title.empty() ? fileName : title;
}

int MediaUtils::getDuration(const std::string &uri)
{
    MetadataExtractor extractor(uri);
    int duration = 0;
    if(extractor.isValid())
    {
        duration = extractor.getInt(METADATA_DURATION);
        MSG_LOG("Duration: ", duration);
    }
    return duration;
}

bool MediaUtils::getVideoFrame(const std::string &videoFilePath, const std::string &imageFilePath)
{
    MetadataExtractor extractor(videoFilePath);
    if(!extractor.isValid())
        return false;

    int videoW = extractor.getInt(METADATA_VIDEO_WIDTH);
    int videoH = extractor.getInt(METADATA_VIDEO_HEIGHT);

    int thumbSize = 0;
    void *thumbnail = nullptr;
    extractor.getFrame(&thumbnail, &thumbSize);

    MSG_LOG("Frame: width = ", videoW, " height = ", videoH, " size = ", thumbSize);

    if(thumbnail)
    {
        const int quality = 90; // JPEG image quality(1 ~ 100)
        int ret = image_util_encode_jpeg((unsigned char *)thumbnail, videoW, videoH, IMAGE_UTIL_COLORSPACE_RGB888, quality, imageFilePath.c_str());
        free(thumbnail);
        return ret != IMAGE_UTIL_ERROR_NONE;

    }
    return false;
}

