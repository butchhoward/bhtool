#include <bhtool/bhtool.hpp>
#include <iostream>

#include <bhtool/stderrred.hpp>


bhtool::commands bhtool_commands = {
     {std::string("bhtool"), bhtool::bhtool}
    ,{std::string("help"), bhtool::last_ditch_usage}
    ,{std::string(STDERRRED_CMD), bhtool::stderrred}
};

const bhtool::commands& bhtool::command_map()
{
    return bhtool_commands;
}

int bhtool::bhtool(int argc, char *argv[])
{
    std::string command;
    command = argc > 1 ? argv[1] : "help";

    auto cmd = bhtool::find_command(command, bhtool::command_map());

    auto code = cmd(--argc, &(argv[1]));

    return code;
}

int bhtool::last_ditch_usage(int /*argc*/, char */*argv*/[])
{
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

bhtool::cmd_function bhtool::find_command(const std::string command_name, commands cmds)
{
    auto cmd = cmds.find(command_name);
    if (cmd != cmds.end())
    {
        return cmd->second;
    }

    return bhtool::last_ditch_usage;
}

