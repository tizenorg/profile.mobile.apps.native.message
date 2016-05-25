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

#ifndef DateLineViewItem_H_
#define DateLineViewItem_H_

#include "ListItem.h"

namespace Msg
{
    class DateLineViewItem
        : public ListItem
    {
        public:
            DateLineViewItem(const std::string &dateLine);
            virtual ~DateLineViewItem();

            std::string getDateLine() const;

        protected:
            void setDateLine(const std::string &dateLine);

        private:
            virtual std::string getText(ListItem &item, const char *part);

        private:
            std::string m_DateLine;
    };
}

#endif /* DateLineViewItem_H_ */
