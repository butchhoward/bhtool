#pragma once

#include <string>
#include <functional>
#include <map>


namespace bhtool
{

typedef std::function<int(int, char *[])> cmd_function;

typedef std::map<std::string, cmd_function> commands;

cmd_function find_command(const std::string command_name, commands cmds);

int last_ditch_usage(int argc, char *argv[]);


}
