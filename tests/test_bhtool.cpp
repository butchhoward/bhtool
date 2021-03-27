#include "gtest/gtest.h"
#include <bhtool/bhtool.hpp>
#include <bhtool/stderrred.hpp>
#include <bhtool/repo.hpp>

using namespace bhtool;

template<typename T, typename... U>
size_t getStdFnAddress(std::function<T(U...)> f) {
    typedef T(fnType)(U...);
    fnType ** fnPointer = f.template target<fnType*>();
    return (size_t) *fnPointer;
}

int some_command_fn(int, char *[]) { return 99; };

TEST( utilities, find_cmd_function_from_command ) {

    Commands cmds = {
        {std::string("some_command"), some_command_fn}
    };

    auto actual = find_command("some_command", cmds);
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&some_command_fn);
}

TEST( utilities, find_cmd_function_invalid_command_gives_usage ) {

    Commands cmds = {
        {std::string("some_command"), some_command_fn}
    };

    auto actual = find_command("some_other_command", cmds);
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&last_ditch_usage);
}

TEST( utilities, help_cmd_is_usage ) {

    auto actual = find_command("help", bhtool::command_map());
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&last_ditch_usage);
}

TEST( utilities, stderrred_cmd_is_stderrred ) {

    auto actual = find_command(stderrred::CMD_NAME, bhtool::command_map());
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&stderrred::stderrred);
}

TEST( utilities, repo_cmd_is_repo ) {

    auto actual = find_command(repo::CMD_NAME, bhtool::command_map());
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&repo::repo);
}

TEST( utilities, venv_cmd_is_not_implemented ) {

    auto actual = find_command("venv", bhtool::command_map());
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&not_implemented_yet);
}

TEST( bhtool, not_implemented_returns_86 ) {

    EXPECT_EQ(not_implemented_yet(0, nullptr), 86 );
}
