#include <iostream>
#include "Server/Server.hpp"

namespace
{
    const int PORT = 4000;
}

int main()
{
    Mud::Server::Server server(PORT);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
