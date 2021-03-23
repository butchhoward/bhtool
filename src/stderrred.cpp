#include <bhtool/stderrred.hpp>
#include <iostream>

#include <bhtool/spc.hpp>

int bhtool::stderrred(int argc, char *argv[])
{
    SPC_PIPE* p = spc_popen(argv[1], &(argv[1]), NULL);
    if (!p)
    {
        std::cerr << "spc_open failed\n";
    }
    char readbuf[80];
    do {
        fgets(readbuf, 80, p->read_fd);
        if(feof(p->read_fd))
        {
            std::cerr << "read eof inner\n";
            break;
        }

        std::cout << readbuf;
    } while(!feof(p->read_fd));
    std::cerr << "done\n";
    spc_pclose(p);
}
