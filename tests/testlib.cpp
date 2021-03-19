#include "gtest/gtest.h"
#include <modern/lib.hpp>

TEST( example, first_two_returned ) {
    std::vector<double> values {1, 2., 3.};
    auto [the_first, the_second] = first_two(values);

    EXPECT_EQ( the_first, 1.0 );
    EXPECT_EQ( the_second, 2.0 );
}
