#include <bhtool/stderrred.hpp>
#include <bhtool/color_streams.h>
#include <iostream>

#include <bhtool/spc.hpp>

const std::string stderrred::CMD_NAME("stderrred");

namespace {

int stderrred_execution(int argc, char *argv[])
{
    (void)argc;

    SPC_PIPE* p = spc_popen(argv[0], &(argv[0]), NULL);
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

}

int stderrred::stderrred(int argc, char *argv[])
{
    if (argc <= 1)
    {
        return bhtool::last_ditch_usage(argc, argv);
    }

    return stderrred_execution(--argc, &(argv[1]));
}
