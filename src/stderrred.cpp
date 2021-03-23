#include <bhtool/stderrred.hpp>
#include <iostream>

#include <bhtool/spc.hpp>

int bhtool::stderrred(int argc, char *argv[])
{
    SPC_PIPE* p = spc_popen(argv[1], &(argv[1]), NULL);
    if (!p)
    {
        std::cerr << "spc_open failed\n";
        return 254;
    }

    char stdout_buf[80];
    char stderr_buf[80];
    bool stderred = false;
    std::cerr << "bhtool::stderrred LOOPing\n";
    do {
        stdout_buf[0] = '\0';
        stderr_buf[0] = '\0';

        fgets(stdout_buf, 80, p->stdout_fd);
        if(!feof(p->stdout_fd))
        {
            // std::cout << stdout_buf;
        }

        fgets(stderr_buf, 80, p->stderr_fd);
        if(!feof(p->stderr_fd))
        {
            if (!stderred)
            {
                stderred = true;
                std::cerr << u"\u001b[31m" << "!!!";
            }
            // std::cerr << stdout_buf;
        }

    } while(!feof(p->stdout_fd) || !feof(p->stderr_fd));

    if (stderred)
    {
        std::cerr << "!!!" << u"\u001b[0m";
    }
    std::cerr << "bhtool::stderrred DONE\n";

    auto exit_code = spc_pclose(p);

    if (exit_code >= 0)
    {
        return exit_code;
    }

    return 255;
}
