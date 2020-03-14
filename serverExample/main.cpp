#include <iostream>
#include "serverclass.h"
int main()
{
    std::cout << "\033]0;" << "Server" << "\007" << "Server" << std::endl;
    ServerClass sc("127.0.0.1", 1337);
    return 0;
}
