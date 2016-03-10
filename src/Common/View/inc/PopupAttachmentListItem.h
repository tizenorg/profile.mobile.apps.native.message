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

#ifndef PopupAttachmentListItem_h_
#define PopupAttachmentListItem_h_

#include "PopupListItem.h"

namespace Msg
{

    /**
     * A class of popup-for attach files
     */
    class PopupAttachmentListItem
        : public PopupCheckListItem
    {
        public:
            PopupAttachmentListItem(PopupList &parent, const std::string &text, const std::string &path,
                    PopupListItemPressedCb cb, void *userData);
            virtual ~PopupAttachmentListItem();

        public:
            const std::string &getFilePath() const;

        private:
            std::string m_FilePath;
    };
}
#endif /* PopupAttachmentListItem_h_ */
