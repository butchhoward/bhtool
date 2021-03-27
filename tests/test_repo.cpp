#include "gtest/gtest.h"
#include <bhtool/repo.hpp>

#include "utils_test.hpp"

using namespace repo;


TEST( repo, handles_args_correctly ) {
    //think about how to restructure things to be able to test this
}

TEST( repo, wip_rebase_is_handled_by_wip_rebase ) {

    auto actual = bhtool::find_command("wip-rebase", repo::command_map());
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&repo::wip_rebase);
}

TEST( repo, help_is_handled_by_repo_usage ) {

    auto actual = bhtool::find_command("help", repo::command_map());
    EXPECT_EQ(getStdFnAddress(actual), (size_t)&repo::usage);
}
