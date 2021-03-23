#include "gtest/gtest.h"
#include <bhtool/bhtool.hpp>
#include <bhtool/stderrred.hpp>

using namespace bhtool;

TEST( something, sometest ) {
    EXPECT_TRUE( true );
}

int some_command_fn(int, char *[]) { return 99; };

TEST( utilities, find_cmd_function_from_command ) {

    commands cmds = {
        {std::string("some_command"), some_command_fn}
    };

    auto actual = find_command("some_command", cmds);
    ASSERT_NE(actual, nullptr);
    EXPECT_EQ( actual(0, nullptr), 99 );
    //could not figure out how to check the function pointer. Tried several variations of
    //  EXPECT_EQ(actual,some_command_fn);
}

TEST( utilities, find_missing_cmd_function_from_command ) {

    commands cmds = {
        {std::string("some_command"), some_command_fn}
    };

    auto actual = find_command("some_other_command", cmds);
    EXPECT_EQ( actual, nullptr );
}
