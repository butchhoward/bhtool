#pragma once

#include <bhtool/bhtool.hpp>



namespace repo {

extern const std::string CMD_NAME;

int repo(int argc, char *argv[]);
const bhtool::Commands command_map();

int usage(int argc, char *argv[]);
int wip_rebase(int argc, char *argv[]);


}
