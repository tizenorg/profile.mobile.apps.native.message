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
 */

#ifndef _MSG_MEDIA_H__
#define _MSG_MEDIA_H__

#include "MsgList.h"
#include <memory>

namespace Msg
{
    class MsgMedia;
    typedef std::shared_ptr<MsgMedia> MsgMediaRef;
    typedef MsgList<MsgMedia> MsgMediaList;
    typedef std::shared_ptr<MsgList<MsgMedia>> MsgMediaListRef;

    /**
     * @brief represents a file to be attached to message.
     */
    class MsgMedia
    {
        public:
            /**
             * @brief Attacment-type.
             */
            enum Type
            {
                UnknownType = 0,    /**< File type unsupported by smil-player, can't be played by smil-player. Attached as unknown filetype.*/
                ImageType,          /**< Image file. Can be played with smil-player.*/
                AudioType,          /**< Audio file. Can be played with smil-player.*/
                VideoType,          /**< Video file. Can be played with smil-player.*/
                TextType,           /**< Text-section of page.*/
                AnimateType,        /**< Animated vector graphics or other animated format supported by smil-player. */
                ImageOrVideoType //TODO: I'm unused enum-item, remove me.
            };

        public:
            virtual ~MsgMedia() {};

            /**
             * @brief Gets type of media.
             * @return media type.
             */
            virtual Type getType() const = 0;

            /**
             * @brief Gets path to file.
             * @return path.
             */
            virtual std::string getFilePath() const = 0;

            /**
             * @brief Gets filename.
             * @return filename.
             */
            virtual std::string getFileName() const = 0;

            /**
             * @brief Sets path to file.
             * @param[in] path.
             */
            virtual void setFilePath(const std::string &path) = 0;

            /**
             * @brief Sets filename.
             * @param[in] filename.
             */
            virtual void setFileName(const std::string &name) = 0;
    };
}

#endif /* _MSG_MEDIA_H__ */
