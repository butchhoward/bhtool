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
    std::cout << "BEGIN repo\n";

    std::string command;
    command = argc > 1 ? argv[1] : bhtool::CMD_NAME_HELP;

    auto cmd = bhtool::find_command(command, repo::command_map());

    auto code = cmd(--argc, &(argv[1]));

    std::cout << "END repo\n";
    return code;
}

int repo::usage(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    std::cout << "Usage: \n"
              << "repo <cmd> [args...]\n"
              << "\n"
              << "Commands:\n"
              << "\twip-rebase (not implemented yet)\n"
              << "\thelp (not implemented yet)\n"
              << "\n";
    return 1;
}

// int repo::wip_merge(int argc, char *argv[])
// {
//     // BASE_BRANCH=${1:-"main"}

//     // git checkout "${BASE_BRANCH}" || return $?
//     // git merge --ff-only wip || return $?
//     // git branch -d wip || return $?
// }

int repo::wip_rebase(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    git_libgit2_init();

    git_repository *gitrepo = nullptr;
    auto error = git_repository_open_ext(&gitrepo, ".", 0, nullptr );
    if (error < 0)
    {
        const git_error *e = git_error_last();
        std::cerr << "Error " << error << "/" << e->klass << ": " << e->message <<  "\n";
        return 1;
    }
    // BASE_BRANCH=${1:-"main"}
    git_reference *ref = nullptr;
    error = git_repository_head(&ref, gitrepo);
    if (error < 0)
    {
        const git_error *e = git_error_last();
        std::cerr << "Error " << error << "/" << e->klass << ": " << e->message <<  "\n";
        return 1;
    }
    char oid_hex[GIT_OID_HEXSZ+1] = GIT_OID_HEX_ZERO;
    const char *refname;
    refname = git_reference_name(ref);

    switch (git_reference_type(ref))
    {
    case GIT_REFERENCE_DIRECT:
        git_oid_fmt(oid_hex, git_reference_target(ref));
        printf("%s [%s]\n", refname, oid_hex);
        break;

    case GIT_REFERENCE_SYMBOLIC:
        printf("%s => %s\n", refname, git_reference_symbolic_target(ref));
        break;

    default:
        fprintf(stderr, "Unexpected reference type\n");
    }
    git_reference_free(ref);

    // git branch wip || return $?
    // git reset --hard "origin/${BASE_BRANCH}" || return $?
    // git checkout wip || return $?
    // git rebase "${BASE_BRANCH}" || return $?
    // # if there are merge conflicts, resolve them and then
    // repo_wip_merge "${BASE_BRANCH}"

    git_repository_free(gitrepo);
    git_libgit2_shutdown();

    return 0;
}
