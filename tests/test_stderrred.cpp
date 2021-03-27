#include "gtest/gtest.h"
#include <bhtool/stderrred.hpp>

//Not sure how to test with forking.
//Will have to think about how to refactor that thing to inject the spc_open parts
//and test around that

TEST( stderrred, sometest ) {
    EXPECT_TRUE( true );
}
