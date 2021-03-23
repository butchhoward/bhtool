#include <bhtool/bhtool.hpp>
#include <iostream>

using namespace bhtool;

int bhtool::last_ditch_usage(int /*argc*/, char *argv[])
{
    std::cerr << "Could not find that command!\n";
    std::cout << "Usage: \n"
              << "\tbhtool <cmd> [args...]\n"
              << "\n"
              << "Commands:\n"
              << "\tstderrred\n"
              << "\trepo (not implemented yet)\n"
              << "\tvenv (not implemented yet)\n"
              << "\tdocker (not implemented yet)\n"
              << "\n";
    return 1;
}

cmd_function bhtool::find_command(const std::string command_name, commands cmds)
{
    auto cmd = cmds.find(command_name);
    if (cmd != cmds.end())
        return cmd->second;

    return last_ditch_usage;
}
