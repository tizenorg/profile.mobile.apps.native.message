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

#ifndef _MSG_PAGE_H__
#define _MSG_PAGE_H__

#include "MsgMedia.h"

namespace Msg
{
    class MsgPage;
    typedef std::shared_ptr<MsgPage> MsgPageRef;
    typedef MsgList<MsgPage> MsgPageList;
    typedef std::shared_ptr<MsgList<MsgPage>> MsgPageListRef;

    /**
     * @brief An abstraction that represents a concept of "page".
     * This is a part of MMS that contains one previewable attachment(video or image) and a number of attachments that have no preview(documents, audio-files etc).
     */
    class MsgPage
    {
        public:
            virtual ~MsgPage();

            /**
             * @brief Non-constant getter of media-attachment list.
             * @return list of media-attachments.
             */
            virtual MsgMediaList &getMediaList() = 0;

            /**
             * @brief Constant getter of media-attachment list.
             * @return read-only media-attachment list.
             */
            const MsgMediaList &getMediaList() const;

            /**
             * @brief Adds media to pages.
             * @return media-structure that was added.
             */
            virtual MsgMedia &addMedia() = 0;

            /**
             * @brief Sets page duration.
             * @param[in] duration page duration in seconds.
             */
            virtual void setPageDuration(int duration) = 0;

            /**
             * @brief Gets page duration.
             * @return duration page duration in seconds.
             */
            virtual int getPageDuration() const = 0;
    };
}

#endif /* _MSG_PAGE_H__ */
