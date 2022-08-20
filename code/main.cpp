#include <iostream>
#include "Server/Server.hpp"
#include "World/World.hpp"

namespace
{
    const int PORT = 4000;
}

int main()
{
    Mud::Logic::World world;

    Mud::Logic::Room room(1, "You are standing in a dark room.  The walls are bare and the air is musty.");
    world.AddRoom(room);

    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
