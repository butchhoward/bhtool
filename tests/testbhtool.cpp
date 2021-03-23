#include "gtest/gtest.h"
#include <bhtool/bhtool.hpp>
#include <bhtool/stderrred.hpp>

using namespace bhtool;

template<typename T, typename... U>
size_t getStdFnAddress(std::function<T(U...)> f) {
    typedef T(fnType)(U...);
    fnType ** fnPointer = f.template target<fnType*>();
    return (size_t) *fnPointer;
}

TEST( something, sometest ) {
    EXPECT_TRUE( true );
}

int some_command_fn(int, char *[]) { return 99; };

TEST( utilities, find_cmd_function_from_command ) {

    commands cmds = {
        {std::string("some_command"), some_command_fn}
    };

    auto actual = find_command("some_command", cmds);
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&some_command_fn);
}

TEST( utilities, find_missing_cmd_function_from_command ) {

    commands cmds = {
        {std::string("some_command"), some_command_fn}
    };

    auto actual = find_command("some_other_command", cmds);
    EXPECT_EQ( actual, nullptr );
}
