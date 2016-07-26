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

#ifndef MsgList_H_
#define MsgList_H_


namespace Msg
{
    /**
     * @brief Generic container for message-objects.
     */
    template<typename T>
    class MsgList
    {
        public:
            typedef T Type;

        public:
            virtual ~MsgList() {}

            /**
             * @brief Constant getter by index..
             * @param[in] i index.
             * @return message-object.
             */
            virtual const T &at(int i) const = 0;

            /**
             * @brief Nonconstant getter by index..
             * @param[in] i index.
             * @return message-object.
             */
            virtual T &at(int i) = 0;

            /**
             * @brief Gets count of message-objects list contains.
             * @return list size.
             */
            virtual int getLength() const = 0;

            inline const T &operator[](int i) const
            {
                return at(i);
            }

            inline const T &operator[](int i)
            {
                return at(i);
            }

            /**
             * @brief Checks if list is empty or not.
             * @return true if list is empty, false otherwise.
             */
            inline bool isEmpty() const
            {
                return getLength() <= 0;
            }
    };
}

#endif /* MsgList_H_ */
