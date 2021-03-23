#include <bhtool/bhtool.hpp>
#include <bhtool/stderrred.hpp>

#include <fmt/format.h>

#include <iostream>


bhtool::commands command_map = {
    {std::string(STDERRRED_CMD), bhtool::stderrred}
};


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    auto cmd = bhtool::find_command(argv[1], command_map);
    if (cmd == nullptr)
    {
        return 2;
    }

    auto code = cmd(--argc, &(argv[1]));

    return code;
}
