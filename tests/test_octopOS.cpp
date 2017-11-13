#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "octopos.h"

BOOST_AUTO_TEST_CASE(getInstance) {
    BOOST_REQUIRE_NO_THROW(octopOS::getInstance());
}
