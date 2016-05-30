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
#include <math.h>
#include <libexif/exif-data.h>
#include <fstream>

using namespace Msg;

namespace
{
    const int QUALITY = 50;
    const unsigned char EXIF_HEADER[] = { 0xff, 0xd8, 0xff, 0xe1 };
    const unsigned int EXIF_HEADER_LEN = sizeof(EXIF_HEADER);
    const unsigned int JPEG_HEADER_OFFSET = 20;
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
        MSG_LOG("Duration msec: ", duration);
    }
    return duration;
}

int MediaUtils::getDurationSec(const std::string &uri)
{
    double sec = ceil(getDuration(uri) / 1000.0);
    int res = sec <= 0 ? 1 : sec;
    MSG_LOG("File: ", uri);
    MSG_LOG("Duration sec: ", res);
    return res;
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

    MSG_LOG("File: ", videoFilePath);
    MSG_LOG("Frame: width = ", videoW, " height = ", videoH, " size = ", thumbSize);

    if(thumbnail)
    {
        const int quality = 90; // JPEG image quality(1 ~ 100)
        int ret = image_util_encode_jpeg((unsigned char *)thumbnail, videoW, videoH, IMAGE_UTIL_COLORSPACE_RGB888, quality, imageFilePath.c_str());
        free(thumbnail);
        return ret == IMAGE_UTIL_ERROR_NONE;

    }
    return false;
}

bool MediaUtils::getFrameSize(const std::string &videoFilePath, int &width, int &height)
{
    MetadataExtractor extractor(videoFilePath);
    if(!extractor.isValid())
        return false;

    width = extractor.getInt(METADATA_VIDEO_WIDTH);
    height = extractor.getInt(METADATA_VIDEO_HEIGHT);
    int orient = extractor.getInt(METADATA_ROTATE);
    if(orient == 90 || orient == 270)
        std::swap(width, height);

    MSG_LOG("File: ", videoFilePath);
    MSG_LOG("Frame: width = ", width, " height = ", height);

    return true;
}

long long MediaUtils::downgradeImageQuality(const std::string &imagePath)
{
    image_util_decode_h decode_h = {};
    image_util_encode_h encode_h = {};
    unsigned char *buffer = nullptr;
    unsigned long width = 0;
    unsigned long height = 0;
    int res = IMAGE_UTIL_ERROR_NONE;
    ExifData *exifData = nullptr;
    unsigned char *exifBuff = nullptr;
    unsigned int exifBuffLen = 0;
    unsigned char *jpegBuff = nullptr;
    unsigned long long int jpegBuffLen = 0;
    std::ofstream outputFile;

    exifData = exif_data_new_from_file(imagePath.c_str());
    if(exifData)
    {
        exif_data_save_data(exifData, &exifBuff, &exifBuffLen);
    }

    res = image_util_decode_create(&decode_h);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_decode_set_input_path(decode_h, imagePath.c_str());
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        image_util_decode_destroy(decode_h);
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_decode_set_output_buffer(decode_h, &buffer);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        image_util_decode_destroy(decode_h);
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_decode_run(decode_h, &width, &height, nullptr);
    image_util_decode_destroy(decode_h);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_encode_create(IMAGE_UTIL_JPEG, &encode_h);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_encode_set_resolution(encode_h, width, height);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        image_util_encode_destroy(encode_h);
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_encode_set_quality(encode_h, QUALITY);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        image_util_encode_destroy(encode_h);
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_encode_set_input_buffer(encode_h, buffer);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        image_util_encode_destroy(encode_h);
        return FileUtils::getFileSize(imagePath);
    }

    res = image_util_encode_set_output_buffer(encode_h, &jpegBuff);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        image_util_encode_destroy(encode_h);
        return FileUtils::getFileSize(imagePath);
    }
    res = image_util_encode_run(encode_h, &jpegBuffLen);
    image_util_encode_destroy(encode_h);
    if(res != IMAGE_UTIL_ERROR_NONE)
    {
        free(buffer);
        free(jpegBuff);
        return FileUtils::getFileSize(imagePath);
    }

    outputFile.open(imagePath.c_str());
    if(!outputFile.is_open())
    {
        MSG_LOG_ERROR("Can't open file for write");
        free(buffer);
        free(jpegBuff);
        return FileUtils::getFileSize(imagePath);
    }
    if(exifData)
    {
        // Write EXIF header
        outputFile.write((char *)EXIF_HEADER, EXIF_HEADER_LEN);
        // Write EXIF block length in big-endian order
        outputFile.put((exifBuffLen+2) >> 8);
        outputFile.put((exifBuffLen+2) & 0xff);
        // Write EXIF data block
        outputFile.write((char *)exifBuff, exifBuffLen);
        // Write JPEG image data, skipping the non-EXIF header
        outputFile.write((char *)jpegBuff + JPEG_HEADER_OFFSET, jpegBuffLen - JPEG_HEADER_OFFSET);
        free(exifBuff);
        exif_data_unref(exifData);
    }
    else
    {
        // Write JPEG image data
        outputFile.write((char *)jpegBuff, jpegBuffLen);
    }
    outputFile.close();
    free(buffer);
    free(jpegBuff);

    return FileUtils::getFileSize(imagePath);
}
