#include "ContactUtils.h"

namespace Msg
{
    namespace Test
    {
        ContactUtils::ContactUtils()
        {
            contacts_connect();
        }

        ContactUtils::~ContactUtils()
        {
            contacts_disconnect();
        }

        ContactUtils &ContactUtils::getInst()
        {
            static ContactUtils inst;
            return inst;
        }

        int ContactUtils::createContact(const std::string &strName, const std::string &strNumber)
        {
            contacts_record_h contact = NULL;
            contacts_record_create(_contacts_contact._uri, &contact);

            contacts_record_h name = NULL;
            contacts_record_create(_contacts_name._uri, &name);
            contacts_record_set_str(name, _contacts_name.first, strName.c_str());
            contacts_record_add_child_record(contact, _contacts_contact.name, name);

            contacts_record_h number = NULL;
            contacts_record_create(_contacts_number._uri, &number);
            contacts_record_set_str(number, _contacts_number.number, strNumber.c_str());
            contacts_record_add_child_record(contact, _contacts_contact.number, number);

            int contact_id = 0;
            contacts_db_insert_record(contact, &contact_id);
            contacts_record_destroy(contact, true);

            return contact_id;
        }

        void ContactUtils::removeContact(int contactId)
        {
        	contacts_db_delete_record(_contacts_contact._uri, contactId);
        }
    }
}
