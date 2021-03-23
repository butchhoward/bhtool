#include <bhtool/bhtool.hpp>
#include <bhtool/stderrred.hpp>

#include <fmt/format.h>

#include <iostream>


bhtool::commands command_map = {
    {std::string(STDERRRED_CMD), bhtool::stderrred}
};


int main(int argc, char *argv[])
{
    for (auto i = 0; i < argc; ++i)
    {
        fmt::print("bhtool: {} - {}\n",  i, argv[i] );
    }

    if (argc < 2)
    {
        return 1;
    }

    auto cmd = command_map.find(std::string(argv[1]));
    if (cmd == command_map.end())
    {
        return 2;
    }

    fmt::print("bhtool calling: {}\n",  cmd->first );
    auto code = cmd->second(--argc, &(argv[1]));

    return code;
}
