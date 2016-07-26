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

#ifndef MsgAddress_h_
#define MsgAddress_h_

#include "MsgList.h"

#include <memory>
#include <string>

namespace Msg
{
    class MsgAddress;
    typedef MsgList<MsgAddress> MsgAddressList;
    typedef std::shared_ptr<MsgList<MsgAddress>> MsgAddressListRef;

    /**
     * @brief Basic abstraction that represents a recipient's address message should be delivered to.
     */
    class MsgAddress
    {
        public:
            /**
             * @brief Type of address.
             */
            enum AddressType
            {
                Phone,              /**< Phone number*/
                Email,              /**< Email address*/
                UnknownAddressType  /**< Unknown address, you get it if phone number format or email-format validations has failed to parce address.*/
            };

            /**
             * @brief A kind of recipient that defines a way how message should be delivered to this address.
             */
            enum RecipientType
            {
                To,                 /**< Primary recipient, messages are sent directly to this address.*/
                Cc,                 /**< "Carbon copy", secondary recipient. This address gets a message as a copy of original message sent to other "TO"-recipient.*/
                Bcc,                /**< "Blind carbon copy" to tertiary recipients who receive the message. The primary and secondary recipients cannot see the tertiary recipients.*/
                UnknownRecipientType
            };

        public:
            virtual ~MsgAddress() {};

            /**
             * @brief Gets address string.
             * @return address string.
             **/
            virtual std::string getAddress() const = 0;

            /**
             * @brief Gets address type.
             * @return address type.
             **/
            virtual AddressType getAddressType() const = 0;

            /**
             * @brief Gets recipient type.
             * @return recipient type.
             **/
            virtual RecipientType getRecipientType() const = 0;

            /**
             * @brief Sets address-string.
             * @param[in] address address string.
             */
            virtual void setAddress(const std::string &address) = 0;

            /**
             * @brief Sets type of address.
             * @param[in] type address type.
             */
            virtual void setAddressType(AddressType type) = 0;

            /**
             * @brief Sets recipient type.
             * @param[in] type recipient type.
             */
            virtual void setRecipientType(RecipientType type) = 0;
    };

}

#endif /* MsgAddress_h_ */
