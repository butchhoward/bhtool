#include "gtest/gtest.h"
#include <bhtool/repo.hpp>

using namespace repo;

TEST( repo, executes_not_implemented ) {

    // ?? How to actually test that it invoked the not implement handler??
    EXPECT_EQ( repo::repo(0, nullptr), 86);
}

