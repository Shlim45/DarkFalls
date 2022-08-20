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

    Mud::Logic::Area area("Tamia City");

    Mud::Logic::Room room1(1, "You are standing in the first dark room.  The walls are bare and the air is musty.");
    Mud::Logic::Room room2(2, "You are standing in the second dark room.  There is a picture of a snow-covered embankment with deer gazing at a log cabin.");
    Mud::Logic::Room room3(3, "You are standing in the third dark room.  There is a picture of dogs shooting pool and smoking cigars hanging on the wall.");

    area.AddRoom(0,0,0, room1);
    area.AddRoom(1,0,0, room2);
    area.AddRoom(2,0,0, room3);

    world.AddArea(area);

    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
