#include <bhtool/stderrred.hpp>
#include <iostream>

int bhtool::stderrred(int argc, char *argv[])
{

    std::cout << "stderrred: ";
    for (auto i = 0; i < argc; ++i)
    {
        std::cout << i << ": '" << argv[i] << "' ";
    }
    std::cout << "\n";

}
