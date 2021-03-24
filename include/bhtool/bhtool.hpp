#pragma once

#include <string>
#include <functional>
#include <map>


namespace bhtool
{

typedef std::function<int(int, char *[])> CmdFunction;

typedef std::map<std::string, CmdFunction> Commands;

CmdFunction find_command(const std::string command_name, Commands cmds);
const Commands& command_map();

int bhtool(int argc, char *argv[]);
int last_ditch_usage(int argc, char *argv[]);


}
