#pragma once

#include <string>
#include <functional>
#include <map>


namespace bhtool {

typedef std::function<int(int, char *[])> cmd_function;

typedef std::map<std::string, cmd_function> commands;

}
