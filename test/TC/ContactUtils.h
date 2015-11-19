#ifndef __ContactUtils_h__
#define __ContactUtils_h__

#include <string>
#include "ContactManager.h"
#include "ContactPersonNumber.h"
#include "contacts.h"
#include <functional>
#include "Logger.h"

namespace Msg
{
    namespace Test
    {
        class ContactUtils
        {
            public:
                static ContactUtils &getInst();

                int createContact(const std::string &strName, const std::string &strNumber);
                void renameContact(int contactId, const std::string &newName);
                std::string getNameById(int contactId) const;
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
