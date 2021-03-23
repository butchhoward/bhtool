#include <bhtool/bhtool.hpp>
#include <bhtool/stderrred.hpp>

#include <fmt/format.h>

#include <iostream>


bhtool::commands command_map = {
    {std::string(STDERRRED_CMD), bhtool::stderrred}
};


int main(int argc, char *argv[])
{
    std::string command;
    if (argc > 1 )
    {
        command = argv[1];
    }

    auto cmd = bhtool::find_command(command, command_map);

    auto subcommand_args = &(argv[1]);
    auto code = cmd(--argc, subcommand_args);

    return code;
}
