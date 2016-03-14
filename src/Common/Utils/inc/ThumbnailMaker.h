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

#ifndef ThumbnailMaker_h_
#define ThumbnailMaker_h_

#include <Evas.h>
#include <string>
#include <unordered_map>
#include "MsgAddress.h"
#include "ContactManager.h"

namespace Msg
{
    class App;

    class ThumbnailMaker
        : public IContactManagerListener
    {
        public:
            typedef size_t ThumbId;
            enum DefaultThumbs
            {
                SingleThumb = 0,
                GroupThumb,
                OwnerThumb,
                MaxDefaultThumb
            };

        public:
            ThumbnailMaker(App &app);
            ~ThumbnailMaker();
            ThumbnailMaker(ThumbnailMaker&) = delete;
            ThumbnailMaker &operator=(ThumbnailMaker&) = delete;

            ThumbId getThumbId(const MsgAddress &address);
            ThumbId getThumbId(const std::string &address);
            ThumbId getThumbId(DefaultThumbs thumb);
            Evas_Object *getThumbById(Evas_Object *parent, ThumbId id);

        private:
            typedef std::unordered_map<std::string, ThumbId> ContactsMap;
            typedef std::unordered_map<ThumbId, Evas_Object*> OriginsMap;

        private:
            void invalidate();
            ThumbId getThumbIdFromFile(const std::string &path);
            Evas_Object *makeFace(Evas_Object *parent, const std::string &path);
            Evas_Object *makeOriginThumb(Evas_Object *parent, const std::string &path);
            Evas_Object *makeDefaultOriginThumb(Evas_Object *parent, const std::string &path);
            const std::string &defaultThumbsToStr(DefaultThumbs thumb);
            std::string defaultThumbsToPath(DefaultThumbs thumb);

            //IContactManagerListener
            virtual void onContactChanged();
        private:
            App &m_App;
            ContactsMap m_ContactsMap;
            OriginsMap m_OriginsMap;
            ThumbId m_CurrentId;
    };
}

#endif /* ThumbnailMaker_h_ */
