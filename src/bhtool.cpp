#include <bhtool/bhtool.hpp>
#include <iostream>

#include <bhtool/stderrred.hpp>


const std::string bhtool::CMD_NAME("bhtool");
const std::string bhtool::CMD_NAME_HELP("help");

const bhtool::Commands bhtool::command_map()
{
    return {
         {bhtool::CMD_NAME, bhtool::bhtool}
        ,{bhtool::CMD_NAME, bhtool::last_ditch_usage}
        ,{stderrred::CMD_NAME, stderrred::stderrred}
        ,{"repo", bhtool::not_implemented_yet}
        ,{"venv", bhtool::not_implemented_yet}
    };
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
              << "\n";
    return 1;
}

bhtool::CmdFunction bhtool::find_command(const std::string command_name, bhtool::Commands cmds)
{
    auto cmd = cmds.find(command_name);
    if (cmd != cmds.end())
    {
        return cmd->second;
    }

    return bhtool::last_ditch_usage;
}

int bhtool::not_implemented_yet(int argc, char *argv[])
{
    std::cout << "That function is not implemented yet.\n";
    return last_ditch_usage(argc, argv);
}