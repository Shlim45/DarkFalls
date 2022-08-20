#include <iostream>
#include "Server/Server.hpp"
#include "World/World.hpp"

namespace
{
    const int PORT = 4000;
    const std::string MUDNAME = "DarkFalls";
    const int MAJOR = 0;
    const int MINOR = 1;
    const int PATCH = 0;
}

int main()
{
    std::cout << Mud::Server::BR_GREENTEXT << MUDNAME << Mud::Server::BR_WHITETEXT << " version "
              << Mud::Server::BR_YELLOWTEXT << MAJOR << "." << MINOR << "." << PATCH << Mud::Server::BR_WHITETEXT
              << " started.\n";

    std::cout << "Initializing World...\n";
    Mud::Logic::World world;

    std::cout << "Loading Areas...\n";
    Mud::Logic::Area area("Tamia City");
    std::cout << "Areas Loaded.\n";

    std::cout << "Loading Rooms...\n";
    Mud::Logic::Room room1(1, "You are standing in the first dark room.  The walls are bare and the air is musty.");
    Mud::Logic::Room room2(2, "You are standing in the second dark room.  There is a picture of a snow-covered embankment with deer gazing at a log cabin.");
    Mud::Logic::Room room3(3, "You are standing in the third dark room.  There is a picture of dogs shooting pool and smoking cigars hanging on the wall.");
    std::cout << "Rooms Loaded.\n";

    area.AddRoom(0,0,0, room1);
    area.AddRoom(1,0,0, room2);
    area.AddRoom(2,0,0, room3);

    world.AddArea(area);
    std::cout << "World initialized." << std::endl;

    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
