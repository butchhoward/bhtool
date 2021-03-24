#include <bhtool/stderrred.hpp>
#include <bhtool/color_streams.h>
#include <iostream>

#include <bhtool/spc.hpp>

namespace {
    const char ansi_reset[]   = "\x1b[0m";
    const char ansi_red[]     = "\x1b[31m";
}

int bhtool::stderrred(int argc, char *argv[])
{
    SPC_PIPE* p = spc_popen(argv[1], &(argv[1]), NULL);
    if (!p)
    {
        std::cerr << "spc_open failed\n";
        return 254;
    }

    char buf[255];

    do {
        buf[0] = '\0';
        if(fgets(buf, sizeof buf, p->stdout_fd))
        {
            std::cout << buf;
        }

        buf[0] = '\0';
        if(fgets(buf, sizeof buf, p->stderr_fd))
        {
            std::cerr << ansi::red << buf << ansi::reset;
        }

    } while(!feof(p->stdout_fd) || !feof(p->stderr_fd));

    auto exit_code = spc_pclose(p);

    if (exit_code >= 0)
    {
        return exit_code;
    }

    return 255;
}
