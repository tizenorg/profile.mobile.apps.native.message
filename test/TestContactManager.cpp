#define BOOST_TEST_DYN_LINK

#include <string>
#include <boost/test/unit_test.hpp>
#include "ContactUtils.h"

using namespace Msg;

BOOST_AUTO_TEST_SUITE(TestContactManager)

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( FindNameSasha )
{
    const std::string strNumber = "0936403503";
    const std::string strName = "Sasha";

    int contactId = Msg::Test::ContactUtils::getInst().createContact(strName, strNumber);

    ContactManager cm;
    ContactPersonNumber item = cm.getContactPersonNumber(strNumber);
    std::string result = item.getDispName();

    BOOST_TEST_MESSAGE("Number is " << strNumber);
    BOOST_TEST_MESSAGE("Name is " << strName);

    BOOST_CHECK_EQUAL(result, strName);

    Msg::Test::ContactUtils::getInst().removeContact(contactId);
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( FindNameVova )
{
    const std::string strNumber = "0501234567";
    const std::string strName = "Vova";

    int contactId = Msg::Test::ContactUtils::getInst().createContact(strName, strNumber);

    ContactManager cm;
    ContactPersonNumber item = cm.getContactPersonNumber(strNumber);
    std::string result = item.getDispName();

    BOOST_TEST_MESSAGE("Number is " << strNumber);
    BOOST_TEST_MESSAGE("Name is " << strName);

    BOOST_CHECK_EQUAL(result, strName);

    Msg::Test::ContactUtils::getInst().removeContact(contactId);
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()
