#include <bhtool/bhtool.hpp>
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[])
{
    auto command = std::filesystem::path(argv[0]).filename();

    auto cmd = bhtool::find_command(command, bhtool::command_map());

    auto code = cmd(argc, &(argv[0]));

    return code;
}
