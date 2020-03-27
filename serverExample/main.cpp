#include <iostream>
#include "serverclass.h"
int main()
{
    std::cout << "\033]0;" << "Server" << "\007" << "Server" << std::endl;
    ServerClass sc("0.0.0.0", 1337);
    return 0;
}
