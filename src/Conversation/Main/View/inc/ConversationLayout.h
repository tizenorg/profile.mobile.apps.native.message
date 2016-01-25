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

#ifndef ConversationLayout_h_
#define ConversationLayout_h_

#include "View.h"

#include <string>

namespace Msg
{
    class ConversationLayout
        : public View
    {
        public:
            enum Mode
            {
                PredictSearch,
                NewMessage,
                Conversation
            };

        public:
            ConversationLayout(Evas_Object *parent);
            virtual ~ConversationLayout();

            void showPredictSearch(bool value);
            void showSelectAll(bool value);

            void setBubble(Evas_Object *layout);
            void setSelectAll(Evas_Object *layout);
            void setPredictSearch(Evas_Object *layout);
            void setMsgInputPanel(Evas_Object *layout);
            void setRecipientPanel(Evas_Object *layout);
            void setRecipientRect(Evas_Object *layout);

        private:
            void create(Evas_Object *parent);
            Evas_Object *createMainLayout(Evas_Object *parent);
            Evas_Object *createRecipientAreaRect(Evas_Object *parent);

        private:
            Evas_Object *m_pLayout;
    };
}

#endif /* ConversationLayout_h_ */
