#include <bhtool/bhtool.hpp>


int main(int argc, char *argv[])
{
    auto cmd = bhtool::find_command(argv[0], bhtool::command_map());

    auto code = cmd(argc, argv);

    return code;
}
