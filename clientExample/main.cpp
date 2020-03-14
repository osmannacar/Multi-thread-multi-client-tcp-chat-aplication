#include <iostream>
#include "clientclass.h"
int main()
{
    std::cout << "\033]0;" << "Client" << "\007" << "Client" << std::endl;
    ClientClass clclas("127.0.0.1", 1337);
    return 0;
}
