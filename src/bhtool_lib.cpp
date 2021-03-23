#include <bhtool/bhtool.hpp>

using namespace bhtool;

cmd_function bhtool::find_command(const std::string command_name, commands cmds)
{
    auto cmd = cmds.find(command_name);
    if (cmd != cmds.end())
        return cmd->second;

    return nullptr;
}
