#ifndef __ContactUtils_h__
#define __ContactUtils_h__

#include <string>
#include "ContactManager.h"
#include "ContactPersonNumber.h"
#include "contacts.h"
#include <functional>

namespace Msg
{
    namespace Test
    {
        class ContactUtils
        {
            public:
                static ContactUtils &getInst();

                int createContact(const std::string &strName, const std::string &strNumber);
                void removeContact(int contactId);

            private:
                ContactUtils();
                ~ContactUtils();
                ContactUtils(ContactUtils&) = delete;
                ContactUtils &operator=(ContactUtils&) = delete;
        };
    }
}

#endif /* __ContactUtils_h__ */
