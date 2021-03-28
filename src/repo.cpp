#include <bhtool/repo.hpp>
#include <bhtool/bhtool.hpp>

#include <git2.h>

#include <iostream>

const std::string repo::CMD_NAME("repo");


const bhtool::Commands repo::command_map()
{
    return {
         {"wip-rebase", repo::wip_rebase}
        ,{bhtool::CMD_NAME_HELP, repo::usage}
    };
}

int repo::repo(int argc, char *argv[])
{
    std::string command;
    command = argc > 1 ? argv[1] : bhtool::CMD_NAME_HELP;

    auto cmd = bhtool::find_command(command, repo::command_map());

    auto code = cmd(--argc, &(argv[1]));

    return code;
}

int repo::usage(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    std::cout << "Usage: \n"
              << "\repo <cmd> [args...]\n"
              << "\n"
              << "Commands:\n"
              << "\twip-rebase (not implemented yet)\n"
              << "\thelp (not implemented yet)\n"
              << "\n";
    return 1;
}

int repo::wip_rebase(int argc, char *argv[])
{

    git_repository *repo = NULL;
    int error = git_repository_open(&repo, ".");
    if (error != 0)
    {
        std::cerr << "Error opening repository\n";
        return 1;
    }
    git_repository_free(repo);

    return repo::usage(argc, argv);
}
